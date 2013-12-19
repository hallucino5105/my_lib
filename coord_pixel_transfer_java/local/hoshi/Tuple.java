package local.hoshi;

import static java.util.Arrays.deepEquals;
import static java.util.Arrays.deepHashCode;


/**
 * �^�v��
 */
public class Tuple {

    /**
     * �y�A
     *
     * @param <A> A�̌^
     * @param <B> B�̌^
     */
    public static class Pair<A,B> {

        private final A a;
        private final B b;

        /**
         * �R���X�g���N�^
         * @param a a
         * @param b b
         */
        public Pair( A a, B b ) {
            this.a = a;
            this.b = b;
        }

        /**
         * a���擾����B
         * @return a
         */
        public final A getA () {
            return a;
        }
        /**
         * b���擾����B
         * @return b
         */
        public final B getB () {
            return b;
        }

        /* �p�����̃N���X��JavaDoc���Q�� */
        @SuppressWarnings("unchecked")
        public boolean equals ( Object obj ) {
            if ( obj == null ) { return false; }
            if ( obj instanceof Pair ) {
                Object[] that = getValues((Pair<A,B>) obj);
                return deepEquals(that, getValues(this));
            }
            return false;
        }
        /* �p�����̃N���X��JavaDoc���Q�� */
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
     * �g���I
     *
     * @param <A> A�̌^
     * @param <B> B�̌^
     * @param <C> C�̌^
     */
    public static class Trio<A,B,C> extends Pair<A,B> {

        private final C c;

        /**
         * �R���X�g���N�^
         * @param a a
         * @param b b
         * @param c c
         */
        public Trio( A a, B b, C c ) {
            super(a,b);
            this.c = c;
        }
        /**
         * c���擾����B
         * @return c
         */
        public final C getC () {
            return c;
        }

        /* �p�����̃N���X��JavaDoc���Q�� */
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
        /* �p�����̃N���X��JavaDoc���Q�� */
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
     * �J���e�b�g
     *
     * @param <A> A�̌^
     * @param <B> B�̌^
     * @param <C> C�̌^
     * @param <D> D�̌^
     */
    public static class Quartet<A,B,C,D> extends Trio<A,B,C> {

        private final D d;

        /**
         * �R���X�g���N�^
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
         * d���擾����B
         * @return d
         */
        public final D getD () {
            return d;
        }

        /* �p�����̃N���X��JavaDoc���Q�� */
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
        /* �p�����̃N���X��JavaDoc���Q�� */
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
