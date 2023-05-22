#include <algorithm>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <locale>
#include <string.h>
#include <utility>
#include <vector>
#include <stdio.h>
#include <ctype.h>

static const std::pair <const std::string, const std::vector <std::string>> vRules[] = 
{
	{
		"hello",
		{
			"How do you do. Please state your problem."
		}
	},
	{
		"computer",
		{
			"Do computers worry you?",
			"What do you think about machines?",
			"Why do you mention computers?",
			"What do you think machines have to do with your problem?"
		}
	},
	{
		"name",
		{
			"I am not interested in names"
		}
	},
	{
		"sorry",
		{
			"Please don't apologize",
			"Apologies are not necessary",
			"What feelings do you have when you apologize"
		}
	},
	{
		"i remember",
		{
			"Do you often think of #~#",
			"Does thinking of #~# bring anything else to mind?",
			"What else do you remember",
			"Why do you recall #~# right now?",
			"What in the present situation reminds you of #~#",
			"What is the connection between me and #~#"
		}
	},
	{
		"do you remember",
		{
			"Did you think I would forget #~#?",
			"Why do you think I should recall #~# now",
			"What about #~#",
			"You mentioned #~#"
		}
	},
	{
		"if",
		{
			"Do you really think its likely that #~#",
			"Do you wish that #~#",
			"What do you think about #~#",
			"Really-- if #~#"
		}
	},
	{
		"i dreamt",
		{
			"Really-- #~#",
			"Have you ever fantasized #~# while you were awake?",
			"Have you dreamt #~# before?",

		}
	},
	{
		"dream about",
		{
			"How do you feel about #~# in reality?"
		}
	},
	{
		"dream",
		{
			"What does this dream suggest to you?",
			"Do you dream often?",
			"What persons appear in your dreams?",
			"Don't you believe that dream has to do with your problem?"
		}
	},
	{
		"my mother",
		{
			"Who else in your family #~#",
			"Tell me more about your family",
		}
	},
	{
		"my father",
		{
			"Your father",
			"Does he influence you strongly?",
			"What else comes to mind when you think of your father?"
		}
	},
	{
		"i want",
		{
			"What would it mean if you got #~#",
			"Why do you want #~#",
			"Suppose you got #~# soon"
		}
	},
	{
		"i am glad",
		{
			"How have I helped you to be #~#",
			"What makes you happy just now",
			"Can you explain why you are suddenly #~#"
		}
	},
	{
		"i am sad",
		{
			"I am sorry to hear you are depressed",
			"I'm sure its not pleasant to be sad"
		}
	},
	{
		"are like",
		{
			"What resemblance do you see between |~| and #~#"
		}
	},
	{
		"is like",
		{
			"In what way is it that |~| is like #~#",
			"What resemblance do you see?",
			"Could there really be some connection?",
			"How?"
		}
	},
	{
		"alike",
		{
			"In what way?",
			"What similarities are there?"
		}
	},
	{
		"same",
		{
			"What other connections do you see?"
		}
	},
	{
		"i was",
		{
			"Were you really?",
			"Perhaps I already knew you were #~#",
			"Why do you tell me you were #~# now?"
		}
	},
	{
		"was i",
		{
			"What if you were #~# ?",
			"Do you thin you were #~#",
			"What would it mean if you were #~#"
		}
	},
	{
		"i am",
		{
			"In what way are you #~#",
			"Do you want to be #~# ?",
		}
	},
	{
		"am i",
		{
			"Do you believe you are #~#",
			"Would you want to be #~#",
			"You wish I would tell you you are #~#",
			"What would it mean if you were #~#"
		}
	},
	{
		"am",
		{
			"Why do you say \"AM?\"",
			"I don't understand that"
		}
	},
	{
		"are you",
		{
			"Why are you interested in whether I am #~# or not?",
			"Would you prefer if I weren't #~#",
			"Perhaps I am #~# in your fantasies"
		}
	},
	{
		"you are",
		{
			"What makes you think I am #~# ?"
		}
	},
	{
		"because",
		{
			"Is that the real reason?",
			"What other reasons might there be?",
			"Does that reason seem to explain anything else?"
		}
	},
	{
		"were you",
		{
			"Perhaps I was #~#",
			"What do you think?",
			"What if I had been #~#"
		}
	},
	{
		"i can't",
		{
			"Maybe you could #~# now",
			"What if you could #~# ?"
		}
	},
	{
		"i feel",
		{
			"Do you often feel #~# ?"
		}
	},
	{
		"i felt",
		{
			"What other feelings do you have?"
		}
	},
	{
		"i*you",
		{
			"Perhaps in your fantasy we >|< each other"
		}
	},
	{
		"why don't you",
		{
			"Should you #~# yourself?",
			"Do you believe I don't #~#",
			"Perhaps I will #~# in good time"
		}
	},
	{
		"yes",
		{
			"You seem quite positive",
			"You are sure",
			"I understand"
		}
	},
	{
		"no",
		{
			"Why not?",
			"You are being a bit negative",
			"Are you saying \"NO\" just to be negative?"
		}
	},
	{
		"someone",
		{
			"Can you be more specific?"
		}
	},
	{
		"everyone",
		{
			"surely not everyone",
			"Can you think of anyone in particular?",
			"Who for example?",
			"You are thinking of a special person"
		}
	},
	{
		"always",
		{
			"Can you think of a specific example",
			"When?",
			"What incident are you thinking of?",
			"Really-- always"
		}
	},
	{
		"what",
		{
			"Why do you ask?",
			"Does that question interest you?",
			"What is it you really want to know?",
			"What do you think?",
			"What comes to your mind when you ask that?"
		}
	},
	{
		"perhaps",
		{
			"You do not seem quite certain"
		}
	},
	{
		"are",
		{
			"Did you think they might not be #~#",
			"Possibly they are #~#"
		}
	},
	{
		"",
		{
			"Very interesting",
			"I am not sure I understand you fully",
			"What does that suggest to you?",
			"Please continue",
			"Go on",
			"Do you feel strongly about discussing such things?"
		}
	}
};

static const std::pair <std::string, std::string> switchViewPointRules[] =
{
	{
		"i",
		"you"	
	},
	{
		"you",
		"i"
	},
	{
		"me",
		"you"
	},
	{
		"am",
		"are"
	}
};

std::string
remove_punctuation(const std::string &sentence)
{
	std::string out{sentence};
	int len = sentence.size();
	
	for (int i = 0; i < len; i++)
	{
		if (ispunct(out[i]))
			out[i] = ' ';
	}

	return out;
}

ssize_t
findPattern (const std::string &sentence, const std::string &pattern)
{
	std::string tmp = " " + sentence + " ";

	auto found = tmp.find(" " + pattern + " ");

	if (found != std::string::npos)
		 return found;

	return -1;
}
std::string
eliza_prompt(const std::string& input)
{
    std::string output = "I DON'T UNDERSTAND";
    std::string rmv = remove_punctuation(input);
    if (rmv.length() > 0) {
        for (const auto& rule : vRules)
        {
            if (rule.first.size() > 0)
            {
                auto special = rule.first.find("*");

                if (special != std::string::npos && (special > 0 && special != (rule.first.size() - 1)))
                {
                    std::string s1 = rule.first.substr(0, special); // starting with 0.. all char up to special
                    std::string s2 = rule.first.substr(special + 1);

                    auto f1 = findPattern(rmv, s1);
                    auto f2 = findPattern(rmv, s2);

                    if (f1 >= 0 && f2 >= 0)
                    {
                        int r = rand() % rule.second.size();
                        output = rule.second[r];

                        auto it = output.find(">|<");
                        if (it)
                        {
                            std::string rep = rmv.substr(f1 + s1.size() + 1, (f2 - 2 - f1 - s1.size()));

                            for (const auto& rsvp : switchViewPointRules)
                            {
                                auto foundSVP = findPattern(rep, rsvp.first);
                                auto dimSVP = rsvp.first.length();

                                if (foundSVP >= 0)
                                {
                                    rep.replace(foundSVP, dimSVP, rsvp.second);
                                }
                            }

                            output.replace(it, 3, rep);
                        }
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    auto found = findPattern(rmv, rule.first);

                    if (found >= 0)
                    {
                        auto dim = rule.first.length() + 1;
                        auto startSec = found + dim;
                        int r = rand() % rule.second.size();
                        output = rule.second[r];

                        auto it = output.find("#~#");
                    
                        if (it != std::string::npos)
                        {

                            std::string r = std::string(input.begin() + startSec, input.end());
                        
                            for (const auto& rsvp : switchViewPointRules)
                            {
                                auto foundSVP = findPattern(r, rsvp.first);
                                auto dimSVP = rsvp.first.length();

                                if (foundSVP >= 0)
                                {
                                    r.replace(foundSVP, dimSVP, rsvp.second);
                                }
                            }

                            output.replace(it, 3, r);
                        }

                        auto it2 = output.find("|~|");

                        if (it2 != std::string::npos)
                        {
                            std::string r2 = std::string(input.begin(), input.begin() + found);
                            if (r2.size() != 0)
                                r2.erase(r2.size() - 1, 1);
                        
                            for (const auto& rsvp2 : switchViewPointRules)
                            {
                                auto foundSVP2 = findPattern(r2, rsvp2.first);
                                auto dimSVP2 = rsvp2.first.length();

                                if (foundSVP2 >= 0)
                                {
                                    r2.replace(foundSVP2, dimSVP2, rsvp2.second);
                                }
                            }

                            output.replace(it2, 3, r2);
                        }

                        break;
                    }
                }
            }

            else
            {
                int rAnswr = rand() % rule.second.size();
                output = rule.second[rAnswr];
                break;
            }
        }

        std::locale loc;
        for (std::string::size_type i = 0; i < output.length(); ++i)
        {
            output[i] = std::toupper(output[i], loc);
        }
    }
    
    return output;
}
