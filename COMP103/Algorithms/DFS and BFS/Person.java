package depthFirstSearch;

public class Person {
	private String name;
    private int birthYear;
    private Person father;
    private Person mother;

    public Person(String name, int birthYear, Person father, Person mother) {
        this.name = name;
        this.birthYear = birthYear;
        this.father = father;
        this.mother = mother;
    }

    public Person getMother() { return mother; }
    public Person getFather() { return father; }

    public String toString() { return name + " - (Born: " + birthYear + ")"; }
}
