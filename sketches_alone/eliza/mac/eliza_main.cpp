#include "eliza.h"

static void
initial_message(std::ostream& cout)
{
	cout << "=====================================" << "\n";
	cout << "                                     " << "\n";
	cout << "            Eliza chatbot            " << "\n";
	cout << "                                     " << "\n";
	cout << "=====================================" << "\n";
	cout << "                                     " << "\n";
	cout << "Welcome. Let's chat!\n";	
}

int main()
{
	initial_message(std::cout);
	while (std::cin.good()) {
	    std::string input;
        std::cout << "ELIZA> ";
        getline(std::cin, input);
        std::string output = eliza_prompt(input);
        std::cout << output << "\n";
	}
}
