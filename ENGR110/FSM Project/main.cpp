// Author: Seb Johnstone
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
#include <cstdint>

// Structs
struct Transition {
  std::string input_signal;
  std::string dest_state;
};

struct State {
  std::string name;
  std::vector<Transition> transitions;
};

struct FSM_out {
   std::vector<State> states;
   std::string accepting_name;
   std::string initial_name;
   State state_in_progress;
};

std::function<void(char c)> currentState;

// Transition acceptability check
bool transitionInput = false;
bool transitionDestination = false;

// Has Name Check
bool stateHasName = false;

std::string inputBuffer; // Buffer that stores any state names parsed inbetween brackets.

FSM_out out{}; // Define and Initialise - automatically sets variables to their default values (instead of garbage) recursively.

std::function<void(char c)> functionStack[10];
uint8_t stackPointer = 0;

// Define FSM States so compiler knows about them.
void initial(char c); // Initial Entrance State
void errorState(char c); // Error State
void openBracket(char c); // Checks for Opening Bracket
void closeBracket(char c); // Checks for Closing Bracket
void checkEarlyCloseBracket(char c); // Checks for an Early close of the bracket.
void takeInput(char c); // Takes the input and stores it in the inputBuffer
void state3(char c); // Initial State Descriptor
void state4(char c); // Accepting State Descriptor
void state5(char c); // State Descriptor
void state6(char c); // State Name Descriptor
void state7(char c); // Transition Input Value Descriptor
void state8(char c); // Transition Description Value Descriptor
void state9(char c); // Transition State

// Implement FSM States
void initial(char c) {
	if (c == 's') {
		functionStack[stackPointer++] = initial; // Return to the initial state after creation.
		functionStack[stackPointer++] = state5; // Second in line is go to State 5 (checking for n or t).
		functionStack[stackPointer++] = checkEarlyCloseBracket; // After Checking Open bracket, check for early close
		currentState = openBracket; // Check for Open Bracket
	} else if (c == 'i') {
		functionStack[stackPointer++] = state3; // Return to state 3 once we get a close bracket.
		functionStack[stackPointer++] = takeInput; // Take input until we get a close bracket (pops and calls next item)
		functionStack[stackPointer++] = checkEarlyCloseBracket; // Check for an early close bracket for the second character
		currentState = openBracket; // Check if there is an opening bracket.
	} else if (c == 'a') {
		functionStack[stackPointer++] = state4; // Return to state 4 once we get a close bracket.
		functionStack[stackPointer++] = takeInput; // Take input until we get a close bracket (pops and calls next item)
		functionStack[stackPointer++] = checkEarlyCloseBracket; // Check for an early close bracket open bracket
		currentState = openBracket; // Check if there is an opening bracket.
	} else {
		// Guard from random inputs
		std::cout << "Input: '" << c << "' is not a valid. Globally, you can have: i{}, a{}, or s{}.\n";
		currentState = errorState;
	}
}

/*
 * Check if the character is a opening bracket.
 * 
 * Else set state to error state.
 * 
 */
void openBracket(char c) {
	if (c == '{') { 
		currentState = functionStack[--stackPointer];
	} else {
		std::cout << "Expected input '{', ' "<< "' was provided\n";
		currentState = errorState;
	}
}

/*
 * Check if the character is a closing bracket.
 * 
 * Else set state to error state.
 * 
 */
void closeBracket(char c) {
	if (c == '}') { 
		currentState = functionStack[--stackPointer];
	} else {
		std::cout << "State was closed early, stray closing brace.\n";
		currentState = errorState;
	}
}

/*
 * Check for early closing bracket, then passes value to next state, and sets next state.
 * 
 * If it's a closing bracket, go to error state.
 * 
 */
void checkEarlyCloseBracket(char c) {
	if (c == '}') {currentState = errorState;}
	else { 
		--stackPointer;
		currentState = functionStack[stackPointer];
		functionStack[stackPointer](c);
	}
}

/**
 * Takes the input, and stores it in the input buffer.
 */
void takeInput(char c) {
	if (c == '}') {
		functionStack[--stackPointer](c);
	} else {
		inputBuffer.push_back(c);
	}
}

// Set Initial State Name
void state3(char c) {
	if (out.initial_name.empty()) {
		out.initial_name = inputBuffer;
		inputBuffer.clear();
		currentState = initial;
	} else {
		std::cout << "Initial State has already been defined.\n";
		currentState = errorState;
	}
}

// Set Accepting State Name
void state4(char c) {
	if (out.accepting_name.empty()) {
		out.accepting_name = inputBuffer;
		inputBuffer.clear();
		currentState = initial;
	} else {
		std::cout << "Accepting state has already defined.\n";
		currentState = errorState;
	}
}

// State describing function, either either n{} or t{}.
void state5(char c) {
	if (c == 'n') {
		functionStack[stackPointer++] = state5;
		functionStack[stackPointer++] = state6; // State to set name of state.
		functionStack[stackPointer++] = takeInput; // State to take input name.
		functionStack[stackPointer++] = checkEarlyCloseBracket; // State to Check if an early bracket has been closed
		currentState = openBracket; // State to check if there is a opening brace.
	} else if (c == 't') {
		functionStack[stackPointer++] = state9;
		functionStack[stackPointer++] = checkEarlyCloseBracket;
		currentState = openBracket;
	} else if (c == '}') {
		// Check if a name was defined
		if (!stateHasName) {
			std::cout << "A state's name was not defined.\n";
			errorState(c);
		}
		stateHasName = false;
		// Clear the stack and return to initial state:
		stackPointer = 0;
		functionStack[0] = NULL;
		currentState = initial;
		
		// Need to move the current state from the state in progress.
		out.states.push_back(out.state_in_progress);
		
		State tempState{};
		out.state_in_progress = tempState;
	} else {
		std::cout << "Unusual input '" << c << "' in state describing function, valid inputs: n{} or t{}\n";
		currentState = errorState;
	}
}

// Set Name: s{n{xxx}}
void state6(char c) {
	if (out.state_in_progress.name == "") {
		out.state_in_progress.name = inputBuffer; // Set current states name.
		inputBuffer.clear(); // Clear the input buffer
		currentState = functionStack[--stackPointer];
	} else { 
		std::cout << "State is already in progress.\n";
		currentState = errorState;
    }
    stateHasName = true;
}

int checkTransitionAvaliable(bool checkingDestState) {
	if (out.state_in_progress.transitions.empty()) {
		return -1;
	} else {
		if (out.state_in_progress.transitions.back().dest_state != "" && out.state_in_progress.transitions.back().input_signal != "") {
			return -1;
		} else if (checkingDestState) {
			if (out.state_in_progress.transitions.back().dest_state == "") {
				return 0;
			}
		} else {
			if (out.state_in_progress.transitions.back().input_signal == "") {
				return 0;
			}
		}
	}
	
	std::cout << "EMPTY TRANSITION PRESENT\n";
	errorState(0);
	return 1;
}

// Set Input Value: t{i{}}
void state7(char c) {
	int val = checkTransitionAvaliable(false);
	if (val == -1) {
		Transition tempTransition;
		tempTransition.input_signal = inputBuffer;
		inputBuffer.clear();
		out.state_in_progress.transitions.push_back(tempTransition);
		currentState = state9;
	} else if (val == 0) {
		out.state_in_progress.transitions.back().input_signal = inputBuffer;
		inputBuffer.clear();
		currentState = state9;
	}
}

// Set Destination Value: t{d{}}
void state8(char c) {
	int val = checkTransitionAvaliable(true);
	if (val == -1) {
		Transition tempTransition;
		tempTransition.dest_state = inputBuffer;
		inputBuffer.clear();
		out.state_in_progress.transitions.push_back(tempTransition);
		currentState = state9;
	} else if (val == 0) {
		out.state_in_progress.transitions.back().dest_state = inputBuffer;
		inputBuffer.clear();
		currentState = state9;
	}
}

// Start/Continue Transition: t{}
void state9(char c) {
	// Checks to see if we are trying to create another i{} or d{} inside this transition
	if ((c == 'i' && transitionInput) || (c == 'd' && transitionDestination)) {
		std::cout << "Under t{} Either two inputs or two destinations have been defined, a transition can only go one place, and only have one input.\n";
		errorState(c);
	} else if (c == 'i') {
		transitionInput = true;
		functionStack[stackPointer++] = state7; // State to set input value of current transition
		functionStack[stackPointer++] = takeInput; // State to take input to inputBuffer.
		functionStack[stackPointer++] = checkEarlyCloseBracket; // State to check for an early bracket closure.
		currentState = openBracket; // Check for an opening bracket.
	} else if (c == 'd') {
		transitionDestination = true;
		functionStack[stackPointer++] = state8; // State to set destination value of current transition.
		functionStack[stackPointer++] = takeInput; // State to take input to inputBuffer.
		functionStack[stackPointer++] = checkEarlyCloseBracket; // State to check for an early bracket closure.
		currentState = openBracket; // Check for an opening bracket.
	} else if (c == '}') {
		if (transitionInput && transitionDestination) {
			transitionDestination = transitionInput = false; // Set booleans to false 
			
			// Create a temporary transition, even if not used, it will be ignored.
			Transition tempTransition;
			tempTransition.dest_state = "";
			tempTransition.input_signal = "";
			out.state_in_progress.transitions.push_back(tempTransition);
			
			currentState = state5; // Return to inside state descriptor.
		} else {
			std::cout << "Stray '}' in transition scope.\n";
			errorState(0);
		}
	} else {
		std::cout << "Input: '" << c << "' is not a valid transition input. Valid inputs: i{} or d{}.\n";
		errorState(0);
	}
}

void errorState(char c) {
	std::cout << "\nInput file is not valid FSM\n";
	exit(1);
}

int SaveCodeFile(std::vector<std::string> &code, std::string fileName){
	std::ofstream codeOutFile(fileName);
	
	// write whole code to the file
	for (unsigned int i = 0 ; i < code.size(); i++) {
		codeOutFile<<code.at(i)<<std::endl;
    }
    codeOutFile.close();
    
    std::cout << "Code saved to " << fileName << ".\n";
    
    return 0;
}

// returns whole specification file as one string
// whitespaces and line ending characters are removed
std::string ReadSpecFile(std::string fileName){
	std::cout<<"Reading specifications file "<<fileName<< ".\n\n";
	std::string out;
	std::ifstream inputFile( fileName );
	std::string line;
	out = "";
	std::string n1;
	
	while(getline(inputFile,line)) {
		// remove all new line characters;
		line.erase( std::remove(line.begin(), line.end(),'\r'), line.end() );
		line.erase( std::remove(line.begin(), line.end(),'\n'), line.end() );
		line.erase( std::remove(line.begin(), line.end(),' '), line.end() );
		out.append(line);
	}
	
	return out;
}

bool checkStatesData() {
	std::cout << "Checking for dead states...\n\n";
	std::vector<std::string> stateNames;
	for (State state : out.states) {
		// Check for duplicate states
		if (std::find(stateNames.begin(), stateNames.end(), state.name) != stateNames.end()) {
			std::cout << "State: " << state.name << " is already defined.\n";
			return true;
		}
		stateNames.push_back(state.name);
	}
	
	// Check if initial state exists
	if (std::find(stateNames.begin(), stateNames.end(), out.initial_name) == stateNames.end()) {
		std::cout << "Initial State: " << out.initial_name << " does not exist.\n";
		return true;
	}
	
	if (std::find(stateNames.begin(), stateNames.end(), out.accepting_name) == stateNames.end()) {
		std::cout << "Accepting State: " << out.accepting_name << " does not exist.\n";
		return true;
	}
	
	for (State state : out.states) {
		for (Transition transition : state.transitions) {
			if (std::find(stateNames.begin(), stateNames.end(), transition.dest_state) == stateNames.end()) {
				if (transition.dest_state != "" && transition.input_signal != "") {
					std::cout << "Transition to state: " << transition.dest_state << " which does not exist.\n";
					return true;
				}
			}
		}
	}
	return false;
}

void addPrepend(std::vector<std::string> &code) {
	code.emplace_back("#include <iostream>\n");
	code.emplace_back("// Define namespace (for easier to read code)");
	code.emplace_back("using namespace std;\n");
	code.emplace_back("// Defining Global Variables");
	code.emplace_back("string state;");
	code.emplace_back("string input;");
	code.emplace_back("int accept = 0;\n");
	code.emplace_back("int Proc(string input) {");
}

void addStateCode(std::vector<std::string> &code, State state, bool first) {
	if (first) {
		code.emplace_back("	if (state == \"" + state.name + "\") {");
	} else {
		code.emplace_back("	else if (state == \"" + state.name + "\") {");
	}
	
	for (Transition transition : state.transitions) {
		if (transition.input_signal == "") continue;
		code.emplace_back("		if (input == \"" + transition.input_signal + "\") {");
		code.emplace_back("			state = \"" + transition.dest_state + "\";");
		code.emplace_back("		}");
	}
	code.emplace_back("	}");
}

void addPostpend(std::vector<std::string> &code) {
	code.emplace_back("	return 0;");
	code.emplace_back("}");
	code.emplace_back("");
	code.emplace_back("int main() {");
	code.emplace_back("	state = \"" + out.initial_name + "\";");
	code.emplace_back("	int result = 0;\n");
	code.emplace_back("	while(1){");
	code.emplace_back("		cout<<\" state=\"<<state<<\" enter signal\"<<endl;");
	code.emplace_back("		cin>>input;");
	code.emplace_back("		result = Proc(input);");
	code.emplace_back("		cout<<\" new state=\"<<state<<endl;");
	code.emplace_back("		if (result == 1) break;");
	code.emplace_back("	}\n");
	code.emplace_back("	cout<<\"FSM has reached accepting state!\"<<endl;");
	code.emplace_back("	return 0;");
	code.emplace_back("}");
}

void build(std::string &input, std::vector<std::string> &code) {
	std::cout << "Traversing input file...\n";
	
	currentState = initial;
	for (char c : input) {
		currentState(c);
	}
	
	if (out.initial_name == "") {
		std::cout << "Initial State definition missing.\n";
		errorState('0');
		return;
	}
	
	if (out.accepting_name == "") {
		std::cout << "Accepting State definition missing.\n";
		errorState('0');
		return;
	}
	
	if (*(currentState.target<void(*)(char)>()) != initial) {
		std::cout << "Unexpected end of file. One or more brackets were left unclosed.\n";
		errorState('0');
	} else if (*(currentState.target<void(*)(char)>()) != errorState) {
		if (checkStatesData()) {
			errorState('0');
		} else {
			std::cout << "Input file is valid. Generating Code...\n";
			addPrepend(code);
			
			bool first = true;
			
			// Loop through states and add them:
			for (State state : out.states) {
				addStateCode(code, state, first);
				first = false;
			}
			
			code.emplace_back("\n	if (state == \"" + out.accepting_name + "\") return 1;\n");
			
			addPostpend(code);
		}
	}
}

int main(void) {
	std::vector<std::string> code;
	std::string inputFileName, outputFileName;
	
	std::cout << "Enter input file name: ";
	std::cin >> inputFileName;
		
	std::string input = ReadSpecFile(inputFileName);
	build(input, code);
	
	std::cout << "Enter output file name: ";
	std::cin >> outputFileName;
	
	SaveCodeFile(code, outputFileName);
	return 0;
}
