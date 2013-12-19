package local.hoshi;

import static java.util.Arrays.deepEquals;
import static java.util.Arrays.deepHashCode;


/**
 * タプル
 */
public class Tuple {

    /**
     * ペア
     *
     * @param <A> Aの型
     * @param <B> Bの型
     */
    public static class Pair<A,B> {

        private final A a;
        private final B b;

        /**
         * コンストラクタ
         * @param a a
         * @param b b
         */
        public Pair( A a, B b ) {
            this.a = a;
            this.b = b;
        }

        /**
         * aを取得する。
         * @return a
         */
        public final A getA () {
            return a;
        }
        /**
         * bを取得する。
         * @return b
         */
        public final B getB () {
            return b;
        }

        /* 継承元のクラスのJavaDocを参照 */
        @SuppressWarnings("unchecked")
        public boolean equals ( Object obj ) {
            if ( obj == null ) { return false; }
            if ( obj instanceof Pair ) {
                Object[] that = getValues((Pair<A,B>) obj);
                return deepEquals(that, getValues(this));
            }
            return false;
        }
        /* 継承元のクラスのJavaDocを参照 */
        public int hashCode () {
            return deepHashCode(getValues(this));
        }
        private static <A,B> Object[] getValues(Pair<A,B> v) {
            return new Object[] {
                v.a,v.b
            };
        }

        @Override
        public String toString() {
            return new StringBuilder().append( this.a )
                .append( "," ).append( this.b ).toString();
        }
    }

    /**
     * トリオ
     *
     * @param <A> Aの型
     * @param <B> Bの型
     * @param <C> Cの型
     */
    public static class Trio<A,B,C> extends Pair<A,B> {

        private final C c;

        /**
         * コンストラクタ
         * @param a a
         * @param b b
         * @param c c
         */
        public Trio( A a, B b, C c ) {
            super(a,b);
            this.c = c;
        }
        /**
         * cを取得する。
         * @return c
         */
        public final C getC () {
            return c;
        }

        /* 継承元のクラスのJavaDocを参照 */
        @SuppressWarnings("unchecked")
        public boolean equals ( Object obj ) {
            if ( obj == null ) { return false; }
            if ( obj instanceof Trio ) {
                Object[] that = getValues((Trio<A,B,C>) obj);
                return deepEquals(that, getValues(this))
                    && super.equals( obj );
            }
            return false;
        }
        /* 継承元のクラスのJavaDocを参照 */
        public int hashCode () {
            return deepHashCode(getValues(this))
                + super.hashCode()*37;
        }
        private static <A,B,C> Object[] getValues(Trio<A,B,C> v) {
            return new Object[] { v.c };
        }

        @Override
        public String toString() {
            return new StringBuilder().append( super.toString() )
                .append( "," ).append( this.c ).toString();
        }
    }

    /**
     * カルテット
     *
     * @param <A> Aの型
     * @param <B> Bの型
     * @param <C> Cの型
     * @param <D> Dの型
     */
    public static class Quartet<A,B,C,D> extends Trio<A,B,C> {

        private final D d;

        /**
         * コンストラクタ
         * @param a a
         * @param b b
         * @param c c
         * @param d d
         */
        public Quartet( A a, B b, C c, D d ) {
            super(a,b,c);
            this.d = d;
        }
        /**
         * dを取得する。
         * @return d
         */
        public final D getD () {
            return d;
        }

        /* 継承元のクラスのJavaDocを参照 */
        @SuppressWarnings("unchecked")
        public boolean equals ( Object obj ) {
            if ( obj == null ) { return false; }
            if ( obj instanceof Quartet ) {
                Object[] that = getValues((Quartet<A,B,C,D>) obj);
                return deepEquals(that, getValues(this))
                    && super.equals( obj );
            }
            return false;
        }
        /* 継承元のクラスのJavaDocを参照 */
        public int hashCode () {
            return deepHashCode(getValues(this))
                + super.hashCode()*37;
        }
        private static <A,B,C,D> Object[] getValues(Quartet<A,B,C,D> v) {
            return new Object[] { v.d };
        }

        @Override
        public String toString() {
            return new StringBuilder().append( super.toString() )
                .append( "," ).append( this.d ).toString();
        }
    }
}
