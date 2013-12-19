#ifndef _JSONPARSE_H_
#define _JSONPARSE_H_


#include <string>
#include <vector>
#include <list>
#include <cassert>
#include <ostream>


namespace json
{
  // utf16->utf8変換
  template<typename T>
  inline void ucs2_to_utf8(char fst, char snd, T &to) {
    if(static_cast<unsigned char>(fst)>=0x08) {
      to += 0xE0 | (0xF0&fst)>>4;
      to += 0x80 | (0x0F&fst)<<2 | (0xC0&snd)>>6;
      to += 0x80 | 0x3F&snd;
    } else if(fst!=0 || static_cast<unsigned char>(snd)>=0x80) {
      to += 0xC0 | (fst&0x7)<<2 | (snd&0xC0)>>6;
      to += 0x80 | 0x3F&snd;
    } else {
      to += snd;
    }
  }
 

  template<typename T>
  inline void surrogate_ucs2_to_utf8(char fst, char snd, char thd, char fth, T &to) {
    to += 0xF0 | 0x03&fst;
    to += 0x80 | ((0xFC&snd)>>2)+0x10;
    to += 0x80 | (0x03&snd)<<4 | (0x03&thd)<<2 | (0xC0&fth)>>6;
    to += 0x80 | 0x3F&fth;
  }
  

  inline bool is_surrogate(char ch) {
    unsigned uc = static_cast<unsigned char>(ch);
    return uc >=0xD8 && uc <= 0xDF;
  }


  // チャンク(メモリブロック)を保持する構造体
  struct chunk
  {
    chunk(unsigned init_size=0x8000) 
      :offset(0), max_chunk_size(init_size) 
    {
      chunks.push_back(new char[max_chunk_size]);
    }
    ~chunk() {
      for(unsigned i=0; i < chunks.size(); i++)
        delete [] chunks[i];
    }
    
    std::vector<char*> chunks;         // チャンクの配列
    unsigned           max_chunk_size; // 一番大きいチャンク(= chunks.back())のサイズ
    unsigned           offset;         // 未使用のメモリ開始位置
  };


  template <class T>
  class allocator
  {
  public:
    /**********/
    /* 型関連 */
    // 型定義
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;
  
    // アロケータをU型にバインドする
    template <class U>
    struct rebind { typedef allocator<U> other; };
  
    /******************/
    /* コンストラクタ */
    allocator(chunk& cnk) : cnk_(cnk) {}

    template <class U> allocator(const allocator<U>& src) 
      : cnk_(const_cast<chunk&>(src.get_chunk())) {}


    /*******************************************/
    /* STLのアロケータクラスに必要なメソッド群 */
    // メモリを割り当てる
    pointer allocate(unsigned num){
      if(cnk_.offset+sizeof(T)*num >= cnk_.max_chunk_size){
        // メモリが足りない場合は、chunkを増やす
        cnk_.max_chunk_size *= 2;
        cnk_.chunks.push_back(new char [cnk_.max_chunk_size]);
        cnk_.offset=0;
        
        return allocate(num); // もう一度allocate
      }
      
      // offsetから、sizeof(T)*num分だけ、メモリを割り当てる
      // XXX: ここでコンストラクタを呼び出すのは間違っている気がする(2009/11/19)
      pointer ptr = new (cnk_.chunks.back()+cnk_.offset) T[num];
      cnk_.offset += sizeof(T)*num;
      return ptr;      
    }

    // 割当て済みの領域を初期化する
    void construct(pointer p, const T& value) {
      new( (void*)p ) T(value);
    }

    // メモリを解放する
    // ※ 何も行わない
    void deallocate(pointer p, unsigned num) {}

    // 初期化済みの領域を削除する
    void destroy(pointer p) { p->~T(); }


    /**************************/
    /* JSONに必要なメソッド群 */
    // 未使用メモリ開始位置(offset)を0にリセットする
    void reset() {
      cnk_.offset = 0;
      if(cnk_.chunks.size() > 1) {
        // チャンクが複数ある場合は、一番最後のもの以外はdeleteする
        char *tmp = cnk_.chunks.back();
        for(unsigned i=0; i < cnk_.chunks.size()-1; i++)
          delete [] cnk_.chunks[i];
        cnk_.chunks.clear();
        cnk_.chunks.push_back(tmp);
      }
    }
    
    // chunkの未使用メモリ開始位置(offset)を設定する
    // JSONのparse_stringのために、特別に用意
    void set_tail(char* ptr) {
      cnk_.offset = ptr-cnk_.chunks.back();
    }

    // 引数のポインタ(ptr)が、あらかじめ確保しているメモリブロックの範囲内かどうかを判定する
    // 範囲外なら、trueを返す
    // ※ 上限のみをチェックしている
    bool out_of_range(char* ptr) {
      return cnk_.chunks.back()+cnk_.max_chunk_size <= ptr;
    }
    
    // 2番目のコンストラクタで使用
    const chunk& get_chunk() const { return cnk_; }    

  private:
    chunk& cnk_;
  };


  class stream
  {
  public:
    stream(const char *beg, const char* end) 
      : cur_(beg), end_(end) {}

    char read()       { assert(!eof()); return *cur_++; }
    void unread()     { --cur_; }
    char prev() const { return cur_[-1]; }
    bool eof()  const { return !(cur_ < end_); }
    const char* ptr() const { return cur_; }

    char read_skip_ws(){
      for(;;++cur_) {
        assert(!eof());
        switch(*cur_){
        case ' ': case '\t': case '\r': case '\n':
          break;
        default:
          return *cur_++;
        }
      }
    }
    
    bool token_end() const { 
      if(eof())
        return true;
      
      switch(*cur_) {
      case ' ': case '\t': case '\r': case '\n': 
      case '}': case ']':  case ',':
        return true;
      }
      return false;
    }
  private:
    const char *cur_;
    const char *end_;
  };


  // valueの型判別用の列挙型
  namespace type {
    enum ENUM {undef,null,boolean,number,string,array,object};
  }


  // jsonのvalueクラス
  class value
  {
  public:
    value(type::ENUM t_, void* p_) :type(t_),p(p_) {}
    value() :type(type::undef) {}

    template<class T>
    T get() { return static_cast<T>(p); }

    type::ENUM type;

  private:
    void *p;
  };


  /***********/
  /* typedef */
  typedef int                                null;
  typedef bool                               boolean;
  typedef double                             number;
  typedef char*                              string;
  typedef const char*                        const_string;
  typedef std::list<value,allocator<value> > array;
  typedef std::pair<const char*,value>       pair;
  
  class object : public std::list<pair,allocator<pair> >
  {
  public:
    object(const allocator<pair>& src)
      : std::list<pair,allocator<pair> >(0,pair(),src) {}

    const value& operator[] (const char* key) const {
      static const value undef;

      for(const_iterator it = begin(); it != end(); ++it) {
        int rlt = strcmp(it->first,key);
        if(rlt==0)
          return it->second;
        else if(rlt < 0)
          return undef;
      }
      return undef;
    }

    value& operator[] (const char* key) {
      for(iterator it = begin(); it != end(); ++it) {
        int rlt = strcmp(it->first,key);
        if(rlt > 0) {
          insert(it,pair(key,value()));
          --it;
          return it->second;
        } else if (rlt==0) {
          return it->second;
        }
      }
      push_back(pair(key,value()));
      return back().second;
    }
  };

  /**********************/
  /* specialized getter */
  template <> array  value::get<array> ()  { return *(static_cast<array*>(p)); }
  template <> object value::get<object> () { return *(static_cast<object*>(p)); }
  template <> number value::get<number> () { return *(static_cast<number*>(p)); }

  // jsonパーサクラス
  class parser
  {
  public:
    parser() :alloc(mem),str_buf(alloc) {}
    parser(unsigned init_chunk_size) 
      :mem(init_chunk_size),alloc(mem),str_buf(alloc) {} 

    // JSONの文字列パース用のクラス
    class tmp_string{
    public:
      tmp_string(allocator<char>& alloc_) 
        :alloc(alloc_) {
        size=0;
        buf = alloc.allocate(0);
      }
      
      void clear() {
        size=0;
        buf = alloc.allocate(0);
      }
      
      void append(const char* beg, const char* end) {
        unsigned n = end-beg;
        check_and_resize(n);
        strncpy(buf+size,beg,n);
        size += n;
      }
      
      void operator+=(char ch) {
        check_and_resize(1);
        buf[size++]=ch;
      }

      char* end() const { return buf+size; }
      char* c_str() {
        buf[size]='\0';
        return buf;
      }
    private:
      void check_and_resize(unsigned num) {
        if(alloc.out_of_range(buf+size+num)) {
          char *tmp = alloc.allocate(num+size);
          strncpy(tmp,buf,size);
          buf = tmp;
        }
      }
      
      char *buf;
      unsigned size;
      allocator<char> &alloc;
    };

    /******************/
    /* parse系の関数群 */
    value parse(const char *beg, const char *end) {
      alloc.reset();
      stream in(beg,end);
      return parse_value(in);
    }
    
  private:
    value parse_number(stream &in) {
      const char* beg=in.ptr()-1;
      
      for(char c=in.prev();; c=in.read()) {
        switch(c){
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case '+': case '-': case '.': case 'e': case 'E':
          if(in.eof())
            goto end;
          break;
          
        default:
          in.unread();
          goto end;
        }
      }
    end:
      
      unsigned size = in.ptr()-beg;
      char* buf = alloc.allocate(size+1);
      strncpy(buf,beg,size);
      buf[size]='\0';
      char* endp;
      double *d = reinterpret_cast<double*>(alloc.allocate(sizeof(double)));
      *d = strtod(buf, &endp);
      assert(endp==buf+size);
      return value(type::number,d);
    }

    value parse_null(stream &in) {
      if(in.read()=='u' && in.read()=='l' &&
         in.read()=='l' && in.token_end()) {
        return value(type::null,0);
      }
      assert(false);
    }
    
    value parse_bool(stream &in, bool maybe_true) {
      if(maybe_true) {
        if(in.read()=='r' && in.read()=='u' &&
           in.read()=='e' && in.token_end()) {
          return value(type::boolean, reinterpret_cast<void*>(true));
        }
      } else {
        if(in.read()=='a' && in.read()=='l' &&
           in.read()=='s' && in.read()=='e' &&
           in.token_end()) {
          return value(type::boolean, reinterpret_cast<void*>(false));
        }
      }
      assert(false);
    }

    // 文字を16進数を解釈して、対応する数値を返す
    inline char char_to_hex(stream &in) {
      switch(in.read()) {
#define MAP(c,n) case c: return n;
#define MAP2(c1,c2,n) case c1: case c2: return n;
        MAP('0',0); MAP('1',1); MAP('2',2); MAP('3',3); MAP('4',4);
        MAP('5',5); MAP('6',6); MAP('7',7); MAP('8',8); MAP('9',9);
        MAP2('a','A',10); MAP2('b','B',11); MAP2('c','C',12); 
        MAP2('d','D',13); MAP2('e','E',14); MAP2('f','F',15);
#undef MAP
#undef MAP2
      default:
        assert(false);
      }
    }

    // 16進数表記の文字を二つ読み込み、数値(8bit)に変換する
    inline char read_hex(stream &in) {
      return char_to_hex(in)<<4|char_to_hex(in);    
    }

    // "\uXXXX"形式の文字列を読み込む(UTF-8に変換する)
    void parse_escaped_utf16_char(stream &in, tmp_string& s){
      char fst = read_hex(in);
      char snd = read_hex(in);
      
      if(is_surrogate(fst)) {
        assert(in.read()=='\\' && in.read()=='u');
        surrogate_ucs2_to_utf8(fst,snd,read_hex(in),read_hex(in),s);
      } else {
        ucs2_to_utf8(fst,snd,s);
      }
    }

    value parse_string(stream &in) {
      str_buf.clear();
      const char* beg=in.ptr();
      for(char c=in.read();; c=in.read()) {
        switch(c) {
        case '\\':
          str_buf.append(beg,in.ptr()-1);
          switch(in.read()) {
          case 'b': str_buf += '\b'; break; case 'f': str_buf += '\f'; break;
          case 't': str_buf += '\t'; break; case 'r': str_buf += '\r'; break;
          case 'n': str_buf += '\n'; break;
          case 'u': parse_escaped_utf16_char(in, str_buf); break;
          default:  str_buf += in.prev(); 
          }
          beg=in.ptr();
          break;
        case '"':
          str_buf.append(beg,in.ptr()-1);
          alloc.set_tail(str_buf.end()+1);
          return value(type::string, str_buf.c_str());
        }
      }
    }
    
    value parse_array(stream &in) {
      array *as = new (alloc.allocate(sizeof(array))) array(0,value(),alloc);
        
      if(in.read_skip_ws()!=']') {
        in.unread();
        do{
          as->push_back(parse_value(in));
        }while(in.read_skip_ws()==',');
        assert(in.prev()==']');
      }
      return value(type::array, as);
    }

    value parse_object(stream &in) {
      object *obj = new (alloc.allocate(sizeof(object))) object(alloc);
      
      if(in.read_skip_ws()!='}') {
        in.unread();
        do{
          assert(in.read_skip_ws()=='"');
          value key = parse_string(in);
          assert(in.read_skip_ws()==':');
          (*obj)[key.get<string>()]=parse_value(in);
        }while(in.read_skip_ws()==',');
        assert(in.prev()=='}');
      }
      return value(type::object, obj);
    }

    value parse_value(stream &in) {
      switch(in.read_skip_ws()) {
      case '{': return parse_object(in);                break;
      case '[': return parse_array(in);                 break;
      case '"': return parse_string(in);                break;
      case 't': return parse_bool(in,true) ;            break;
      case 'f': return parse_bool(in,false);            break;
      case 'n': return parse_null(in);                  break;
      default:  return parse_number(in);                break;
      }
    }

    private:
    chunk mem;
    allocator<char> alloc;
    tmp_string      str_buf;
  };
  
  namespace {
    void escape_output(const_string s, std::ostream& out) {
      bool need_escape=false;
      for(const_string c=s; *c != '\0'; ++c)
        switch(*c){
        case '\\': case '"': 
          need_escape=true;
          goto end;
        }
    end:
      out << '"';
      if(need_escape) {
        for(;*s != '\0'; ++s) {
          switch(*s){
          case '\\': case '"':
            out << '\\';
          }
          out << *s;
        }
      } else {
        out << s;
      }
      out << '"';
    }
  }

  void encode(value val, std::ostream& out) {
    switch(val.type) {
    case type::null:    out << "null";                            break;
    case type::boolean: out << val.get<boolean>()?"true":"false"; break;
    case type::number:  out << val.get<number>();                 break;
    case type::string:  escape_output(val.get<string>(),out);     break;

    case type::array:
      {
        const array as = val.get<array>();
        out <<"[";
        for(array::const_iterator it = as.begin(); it != as.end(); ++it) {
          if(it!=as.begin())
            out << ",";
          encode(*it, out);
        }
        out <<"]";
      }
      break;
      
    case type::object:
      {
        const object obj = val.get<object>();
        out <<"{";
        for(object::const_iterator it=obj.begin(); it != obj.end(); ++it) {
          if(it!=obj.begin())
            out <<',';

          escape_output(it->first,out);
          out <<":";
          encode(it->second,out);
        }
        out <<"}";
      }
    }
  }
}


#endif

