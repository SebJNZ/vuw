package depthFirstSearch;

import java.util.ArrayDeque;

public class App {
	/**
	 * DFS - Depth-First Search - Pre order
	 * Recursively prints family tree visiting the root node, 
	 * then traversing sub-trees from left to right.
	 */
    public void printAllPre(Person p) {
        if (p != null) {
            System.out.println(p);
            printAllPre(p.getFather());
            printAllPre(p.getMother());
        }
    }

    /**
     * DFS - Depth-First Search - Post Order
     * Recursively prints family trees by traversing the sub-trees from 
     * left to right, then visits the root node.
     */
    public void printAllPost(Person p) {
        if (p != null) {
            printAllPost(p.getFather());
            printAllPost(p.getMother());
            System.out.println(p);
        }
    }

    /**
     * DFS - Depth-First Search - In order
     * Recursively prints family trees by traversing the left sub-trees, 
     * then visiting the root node, then traversing right sub-trees.
     */
    public void printAllInOrder(Person p) {
        if (p != null) {
            printAllInOrder(p.getFather());
            System.out.println(p);
            printAllInOrder(p.getMother());
        }
    }
    
    /**
     * BFS - Breadth-First Search - Level Order
     * Prints family trees by visiting each node at each level of the tree from top (root)
     * to bottom and left to right.
     */
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

    /**
     * Main method -> Creates an app instance, family tree, then calls the selected
     * search method on the family tree.
     */
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