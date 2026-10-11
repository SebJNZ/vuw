# FSM Project
## Automated FSM Code Generator
We are provided an input file, this input file describes an FSM in a form of **Markup Language**, the language uses the following notation:

**s{ ... }** - State Describer. Name, and transitions are defined inside this.
**s{ n{xxx}... }** - Describes the name of the state
**s{ n{xxx} t{...} }** - Describes the states transitions
**s{ n{xxx} t{ i{...}... } }**- Describes the input signal for the transition to occur.
**s{ n{xxx} t{ i{...} d{...} } }** - Describes the destination state from the input
**i{xxx}** - Defines the initial state
**a{xxx}** - Defines the accepting state.

Example code - if we get a 1, we are in the accepting state
```
s{ t{ i{1} d{s1} } n{s0} t{ i{0} d{s0} } }
i{s0}
s{ n{s1}  }
a{s1}
```

![[example.svg]]

This should generate this `C++` code:
```
#include <iostream>

// Define namespace (for easier to read code)
using namespace std;

// Defining Global Variables
string state;
string input;
int accept = 0;

int Proc(string input) {
	if (state == "s0") {
		if (input == "1") {
			state = "s1";
		}
		if (input == "0") {
			state = "s0";
		}
	}
	else if (state == "s1") {
	}

	if (state == "s1") return 1;

	return 0;
}

int main() {
	state = "s0";
	int result = 0;

	while(1){
		cout<<" state="<<state<<" enter signal"<<endl;
		cin>>input;
		result = Proc(input);
		cout<<" new state="<<state<<endl;
		if (result == 1) break;
	}

	cout<<"FSM has reached accepting state!"<<endl;
	return 0;
}

```

## How does it work?
This program is a ACG FSM so it is built with a base of an FSM, which traverses the input reading each character, transitioning the language code into data structures: 
- `Transition` used inside the `State` struct, defines the input signal and destination state. the `State` struct also contains the name of the state.
- `FSM_out` struct contains each of the `State` structs, the name of the accepting state and initial state, and has a specific `State` struct used when traversing the input.

These data structures are filled by the FSM which uses Function Pointers, and my own implementation of a `Stack` (I realised afterwards there was a built-in library for this...), you start at the initial state (as seen in the below diagram), and traverse the each state, if it gets an unusual input, it figures out the problem and prints it to the user, and exits.

## FSM of my program:![[final.svg]]

