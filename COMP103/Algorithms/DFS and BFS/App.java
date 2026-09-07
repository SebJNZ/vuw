package depthFirstSearch;

import java.util.ArrayDeque;

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
    
    public void printLevelOrder(Person root) {
    	if (root != null) {
    		ArrayDeque<Person> todo = new ArrayDeque<>();
    		todo.offer(root);
    		
    		while(!todo.isEmpty()) {
    			Person p = todo.pop();
    			
    			System.out.println(p);
    			if (p.getFather() != null) { todo.offer(p.getFather()); }
    			if (p.getMother() != null) { todo.offer(p.getMother()); }
    		}
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

        app.printLevelOrder(me);
    }
}
