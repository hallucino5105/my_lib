#!/usr/bin/env python
# coding: utf-8


class DBAccess:
    DatabaseType = {
            "MySQL": "MySQL",
            "PostgreSQL": "PostgreSQL",
            }

    ResultType = {
            "ListResult": "ListResult",
            "DictResult": "DictResult",
            }

    class DatabaseConnectionError(Exception):
        def __init__(self, msg="Lost database connection"):
            Exception.__init__(self, msg)

    @staticmethod
    def value_transform_handler():
        def handler(obj):
            import datetime
            import decimal

            if isinstance(obj, datetime.datetime):
                return obj.strftime("%Y-%m-%d %H:%M:%S")
            elif isinstance(obj, datetime.date):
                return obj.strftime("%Y-%m-%d")
            elif isinstance(obj, decimal.Decimal):
                return float(obj)
            else:
                return None

        return handler

    def __init__(self, **arg):
        self.database_type = arg.get("dbtype") or DBAccess.DatabaseType["MySQL"]
        self.host          = arg.get("host") or ""
        self.username      = arg.get("username") or ""
        self.password      = arg.get("password") or ""
        self.database      = arg.get("database") or ""
        self.connection    = arg.get("connection") if arg.get("connection") != None else True
        self.result_type   = arg.get("restype") or DBAccess.ResultType["DictResult"]

        if self.database_type == DBAccess.DatabaseType["MySQL"]:
            self.port        = int(arg.get("port") or 3306)
            self.charset     = arg.get("charset") or "utf8"
            self.use_unicode = arg.get("use_unicode") or True
        if self.database_type == DBAccess.DatabaseType["PostgreSQL"]:
            self.port = int(arg.get("port") or 5432)

        self.db = None
        self.cur = None

        if self.connection:
            self.connect()

    def connect(self):
        self.close()

        if self.database_type == DBAccess.DatabaseType["MySQL"]:
            from MySQLdb import connect as mysql_connect
            from MySQLdb.cursors import Cursor as mysql_cursor, DictCursor as mysql_dict_cursor

            self.db = mysql_connect(
                    host=self.host,
                    port=self.port,
                    user=self.username,
                    passwd=self.password,
                    db=self.database,
                    charset=self.charset,
                    use_unicode=self.use_unicode)

            if self.result_type == DBAccess.ResultType["ListResult"]:
                self.cur = self.db.cursor(mysql_cursor)
            elif self.result_type == DBAccess.ResultType["DictResult"]:
                self.cur = self.db.cursor(mysql_dict_cursor)
        elif self.database_type == DBAccess.DatabaseType["PostgreSQL"]:
            from pgdb import connect as pgdb_connect

            self.db = pgdb_connect(
                    host="%s:%d" % (self.host, self.port),
                    user=self.username,
                    password=self.password,
                    database=self.database)

            if self.result_type == DBAccess.ResultType["ListResult"]:
                self.cur = self.db.cursor()
            elif self.result_type == DBAccess.ResultType["DictResult"]:
                self.cur = PGDB_DictCursor(self.db.cursor())

    def close(self):
        if self.cur:
            self.cur.close()
            self.cur = None

        if self.db:
            self.db.close()
            self.db = None

    def check_connection(self):
        if not self.db or not self.cur:
            raise DBAccess.DatabaseConnectionError

    def execute(self, query, autocommit=True):
        self.check_connection()

        self.cur.execute(query)
        if autocommit:
            self.commit()

        return self.cur

    def fetchall(self):
        self.check_connection()
        return self.cur.fetchall()

    def commit(self):
        self.check_connection()
        self.db.commit()

    def change_curtype(self, curtype):
        self.close()
        self.curtype = curtype
        self.connect()

    def switch_database(self, db):
        self.check_connection()

        if self.database_type == DBAccess.DatabaseType["MySQL"]:
            self.cur.execute("use %s" % db)
            self.database = db
        elif self.database_type == DBAccess.DatabaseType["PostgreSQL"]:
            self.database = db
            self.connect()

    def show_databases(self):
        self.check_connection()

        if self.database_type == DBAccess.DatabaseType["MySQL"]:
            ret = self.execute("show databases").fetchall()
        elif self.database_type == DBAccess.DatabaseType["PostgreSQL"]:
            ret = self.execute("""
                select datname from pg_database 
                where datistemplate = false order by datname
                """).fetchall()

        return [ r.values()[0].encode("utf-8") for r in ret ]

    def show_tables(self, schema="public"):
        self.check_connection()

        if self.database_type == DBAccess.DatabaseType["MySQL"]:
            ret = self.execute("show tables").fetchall()
            return [ ".".join([r.values()[0]]).encode("utf-8") for r in ret ]
        elif self.database_type == DBAccess.DatabaseType["PostgreSQL"]:
            ret = self.execute("""
                select table_catalog, table_schema, table_name
                from information_schema.tables
                where table_type = 'BASE TABLE'
                and table_catalog = '%s'
                and table_schema = '%s'
                order by table_schema, table_name
                """ % (self.database, schema)).fetchall()

            return [ u".".join([r["table_schema"], r["table_name"]]).encode("utf-8") for r in ret ]

    def show_create_table(self, table, schema="public"):
        self.check_connection()

        if self.database_type == DBAccess.DatabaseType["MySQL"]:
            return self.execute("show create table %s" % table).fetchall()[0]["Create Table"].encode("utf-8")
        elif self.database_type == DBAccess.DatabaseType["PostgreSQL"]:
            self.execute(DBAccess.__pgsql_show_create_table_query, autocommit=False)
            self.execute("select show_create_table('%s', '%s')" % (table, schema), autocommit=False)
            self.commit()
            return self.fetchall()[0]["show_create_table"].encode("utf-8")

    def show_field_list(self, table, schema="public"):
        self.check_connection()

        if self.database_type == DBAccess.DatabaseType["MySQL"]:
            fields = list(self.execute("desc %s" % table).fetchall())
            new_fields = []

            for f in fields:
                new_f = dict()

                for k, v in f.items():
                    if k == "Extra" or k == "Key":
                        del f[k]
                        continue

                    new_f[k.lower()] = str(v)

                new_fields.append(new_f)

            return new_fields
        elif self.database_type == DBAccess.DatabaseType["PostgreSQL"]:
            return self.execute("""
                select
                    column_name as field,
                    case when data_type = 'USER-DEFINED' then udt_name
                        else data_type
                        end as type,
                    is_nullable as null,
                    column_default as default
                from information_schema.columns
                where table_catalog = '%s' and table_schema = '%s' and table_name = '%s'
                order by ordinal_position
                """ % (self.database, schema, table)).fetchall()

    __pgsql_show_create_table_query = """
        CREATE OR REPLACE FUNCTION show_create_table(varchar /*table*/, varchar /*schema*/) returns text as $$
        /*
         * show create table for PostgreSQL v0.61
         *   2010.9.3 Tadashi Murakami
         *
         * Bug reports are welcome:
         *   http://sourceforge.jp/projects/pgshowcreatetbl/
         *
         * Licensed under the Apache License, Version 2.0.
         * You may obtain a copy of the License at
         *   http://www.apache.org/licenses/LICENSE-2.0
         */
        SELECT CASE
          WHEN tb.relkind = 'r' THEN
          (/*0*/
          SELECT 'CREATE TABLE "' || ns.nspname || '"."' || $1 || '" (' || chr(10) || array_to_string
            (/*1,2*/ARRAY(
            SELECT '  "'||"Column" ||'" '|| "Type" || "Modifiers" || "Index" FROM
              (/*3*/
              SELECT at.attnum, ns.nspname as schema, tb.relname as table, at.attname as "Column",
                     /* Type */
                     CASE WHEN at.attinhcount<>0 OR at.attisdropped THEN null
                          ELSE
                          CASE WHEN tp.typname = 'int2' THEN 'smallint'
                               WHEN tp.typname = 'int4' THEN 'integer'
                               WHEN tp.typname = 'int8' THEN 'bigint'
                               WHEN tp.typname = 'float4' THEN 'real'
                               WHEN tp.typname = 'float8' THEN 'double precision'
                               WHEN tp.typname = 'bpchar' THEN 'char'
                               ELSE tp.typname
                           END ||
                          CASE WHEN at.attlen >= 0        THEN ''
                               WHEN at.atttypmod < 4      THEN ''
                               WHEN tp.typname<>'numeric' THEN '('|| at.atttypmod-4 ||')'
                               WHEN (at.atttypmod & 65535) = 4 THEN '('|| (at.atttypmod >> 16) ||')'
                               ELSE '('|| (at.atttypmod >> 16) ||','|| (at.atttypmod & 65535)-4 ||')'
                           END
                      END as "Type",
                     /* Modifiers */
                     CASE WHEN at.attnotnull THEN ' not null' ELSE ''
                      END ||
                     CASE WHEN ad.adbin IS NULL THEN ''
                          ELSE ' default ' || pg_get_expr(ad.adbin, tb.oid)
                      END as "Modifiers",
                     /* one-column Index */
                     CASE WHEN ix.indexrelid IS NULL THEN ''
                          ELSE
                          CASE WHEN ix.indisprimary THEN ' PRIMARY KEY'
                               WHEN ix.indisunique  THEN ' UNIQUE'
                               ELSE ' /* index */'
                           END ||
                          CASE WHEN am.amname <> 'btree' THEN ' /*'||am.amname||'*/' ELSE ''
                           END
                          || ' /* '|| ic.relname ||' */'
                      END as "Index"
              --     , tb.*, at.*, tp.*
                FROM pg_attribute at
              --
              -- for Type, Modifiers
              --
               INNER JOIN pg_type      tp ON at.atttypid = tp.oid
                LEFT OUTER JOIN pg_attrdef ad ON ad.adrelid = tb.oid AND ad.adnum = at.attnum
              --
              -- for one-column Index
              --
                LEFT OUTER JOIN pg_index ix ON ix.indrelid = tb.oid AND ix.indnatts = 1 AND at.attnum = ix.indkey[0]
                LEFT OUTER JOIN pg_class ic ON ix.indexrelid = ic.oid
                LEFT OUTER JOIN pg_am    am ON ic.relam = am.oid
              --
               WHERE tb.oid = at.attrelid
                 AND at.attnum >= 1
              )/*3*/ as columns ORDER BY attnum
            ), ','||chr(10))/*2,1*/
          --
          --
          -- multi-columns Index
          --
          --
          ||(/*1*/
            SELECT
            CASE WHEN count(*) = 0 THEN ''
                 ELSE ','||chr(10)||'  '|| array_to_string
              (/*2,3*/ARRAY(
              SELECT CASE WHEN indisprimary THEN 'PRIMARY KEY ' ELSE 'UNIQUE ' END
                  || substr(indexdef, strpos(indexdef, '('), strpos(indexdef, ')') - strpos(indexdef, '(') + 1)
                  || ' /* '||index||' */'
                FROM
                (/*4*/
                SELECT ic.relname as index, ns.nspname as schema, tb.relname as table -- , ia.attname, ia.attnum
                     , ix.indnatts, ix.indisunique, ix.indisprimary, am.amname
                     , ix.indkey
                     , pg_get_indexdef(ic.oid) as indexdef
                --     , pg_get_expr(ix.indexprs, ix.indrelid), pg_get_expr(ix.indpred, ix.indrelid)
                  FROM pg_index ix
                 INNER JOIN pg_class ic ON ix.indexrelid = ic.oid
                 INNER JOIN pg_am    am ON ic.relam = am.oid
                --  LEFT OUTER JOIN pg_attribute ia ON ia.attnum = any(ix.indkey) AND ia.attrelid = tb.oid
                 WHERE ix.indrelid = tb.oid
                   AND ix.indnatts > 1
                   AND (ix.indisprimary OR ix.indisunique)
                )/*4*/ as def ORDER BY indisprimary desc, index), ','||chr(10)
              )/*3,2*/
             END
            -- copy from the above conditions
              FROM pg_index ix
             WHERE ix.indrelid = tb.oid
               AND ix.indnatts > 1
               AND (ix.indisprimary OR ix.indisunique)
            )/*1*/ || chr(10) || ')'
          )/*0*/
          WHEN tb.relkind = 'v' THEN
               'CREATE VIEW "' || ns.nspname || '"."' || $1 || '" AS (' || chr(10)
               || trim(trailing ';' from pg_get_viewdef(tb.oid)) || chr(10) || ')'
          ELSE '/* pg_class.relkind='''||tb.relkind||''' */'
           END
          as show_create_table
          FROM pg_class tb
         INNER JOIN pg_namespace ns ON tb.relnamespace = ns.oid
         WHERE tb.relname = $1
           AND
          (/*0*/
            ($2 IS NOT NULL AND ns.nspname = $2)
              OR
            (/*1,2*/$2 IS NULL AND ns.nspname = (
            SELECT ns.nspname
            --  FROM pg_class tb, pg_namespace ns
            -- WHERE tb.relname = $1
            --   AND tb.relnamespace = ns.oid
             WHERE strpos(replace(current_setting('search_path'), '"$user"', current_user), ns.nspname) =
              (/*3*/
              SELECT MIN(strpos(replace(current_setting('search_path'), '"$user"', current_user), ns2.nspname))
                FROM pg_class tb2, pg_namespace ns2
               WHERE tb2.relname = $1
                 AND tb2.relnamespace = ns2.oid
              )/*3*/
            ))/*2,1*/
          )/*0*/
        $$ language sql;
        """


# License "Jeethu Rao" jeethu@jeethurao.com 
# reference from http://jeethurao.com/blog/?p=4
class PGDB_DictCursor(object):
    def __init__(self, cursor):
        object.__setattr__(self, '__cursor', cursor)
        object.__setattr__(self, '__dirty', True)
        object.__setattr__(self, '__description', [])

    def __getattribute__(self, x):
        try:
            return object.__getattribute__(self, x)
        except AttributeError:
            cursor = object.__getattribute__(self, '__cursor')
            return getattr(cursor, x)

    def __setattr__(self, x, v):
        setattr(self.__cursor, x, v)

    def fetchone(self):
        cursor = object.__getattribute__(self, '__cursor')
        dirty = object.__getattribute__(self, '__dirty')
        r = cursor.fetchone()

        if dirty is True:
            description = [x[0] for x in cursor.description]
            object.__setattr__(self, '__description', description)
            object.__setattr__(self, '__dirty', False)
        else:
            description = object.__getattribute__(self, '__description')
        if r is not None:
            return dict(zip(description, r))

        return r

    def fetchall(self):
        cursor = object.__getattribute__(self, '__cursor')
        dirty = object.__getattribute__(self, '__dirty')
        l = cursor.fetchall()

        if dirty is True:
            description = [x[0] for x in cursor.description]
            object.__setattr__(self, '__description', description)
            object.__setattr__(self, '__dirty', False)
        else:
            description = object.__getattribute__(self, '__description')
        for count, x in enumerate(l):
            l[count] = dict(zip(description, x))

        return l

    def execute(self, *args, **kwargs):
        object.__setattr__(self, '__dirty', True)
        cursor = object.__getattribute__(self, '__cursor')
        return cursor.execute(*args, **kwargs)

    def __iter__(self):
        return self

    def next(self):
        x = self.fetchone()

        if x is None:
            raise StopIteration

        return x

