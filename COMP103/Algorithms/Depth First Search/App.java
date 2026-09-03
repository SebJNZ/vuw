package depthFirstSearch;

public class App {
    public void printAllPre(Person p) {
        if (p != null) {
            System.out.println(p);
            printAllPre(p.getFather());
            printAllPre(p.getMother());
        }
    }

    public void printAllPost(Person p) {
        if (p != null) {
            printAllPost(p.getFather());
            printAllPost(p.getMother());
            System.out.println(p);
        }
    }

    public void printAllInOrder(Person p) {
        if (p != null) {
            printAllInOrder(p.getFather());
            System.out.println(p);
            printAllInOrder(p.getMother());
        }
    }

    public static void main(String[] args) {
        App app = new App();

        Person me = new Person("A", 2001, 
        		// Father of A
        		new Person("D", 1970, 
        				// Father of D
        				new Person("V", 1944, null, null), 
        				// Mother of D
        				new Person("J", 1943, null, null)),
        		// Mother of A
        		new Person("N", 1971, 
        				// Unknown Father
        				null,
        				// Mother of N
        				new Person("S", 1941, 
        						// Unknown Father
        						null,
        						// Mother of S
        						new Person("H", 1921, null, null))
        		));

        app.printAllInOrder(me);
    }
}