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
			"How do you do. Please state your problem.",
			"Hi.  What seems to be your problem ?"
		}
	},
	{
		"computer",
		{
			"Do computers worry you?",
			"What do you think about machines?",
			"Why do you mention computers?",
			"What do you think machines have to do with your problem?",
			"Don't you think computers can help people ?",
			 "What about machines worrys you ?",
			 "What do you think about machines ?"
				
				
		}
	},
	{
		"name",
		{
			"I am not interested in names",
			"I've told you before, I don't care about names -- please continue."
		}
	},
	{
		"sorry",
		{
			"Please don't apologize",
			"Apologies are not necessary",
			"What feelings do you have when you apologize",
			"I've told you that apologies are not required"
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
			"I am sorry to hear you are sad",
			"I'm sure its not pleasant to be sad",
			    "Do you think that coming here will help you not to be sad ?",
			    "Can you explain what made you sad ?"
		}
	},

	{
		"i am happy",
		{
		    "How have I helped you to be happy ?",
		    "Has your treatment made you happy?",
		    "What makes you happy just now ?",
		    "Can you explan why you are suddenly happy ?"
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
			"What similarities are there?",
			    "What resemblence do you see ?",
			    "What does that similarity suggest to you ?",
			    "What other connections do you see ?",
			    "What do you suppose that resemblence means ?",
			    "What is the connection, do you suppose ?",
			    "Could here really be some connection ?",
			    "How ?"
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
			"Were you #~# ?",
			"What does #~# suggest to you ?"

		}
	},
	{
		"was you",
		{
		      "Would you like to believe I was #~# ?",
		      "What suggests that I was #~# ?",
		      "What do you think ?",
		      "Perhaps I was #~# .",
		      "What if I had been #~# ?"
		}
	},
	
	{
		"i desire",
		{
	        "What would it mean to you if you got #~# ?",
	        "Why do you want #~# ?",
	        "Suppose you got #~# soon ?",
	        "What if you never got #~# ?",
	        "What would getting #~# mean to you ?",
	        "What does wanting #~# have to do with this discussion ?"
		}
	},

	{
		"i believe",
		{
	        "Do you really think so ?"
	        "But you are not sure you believe."
	        "Do you really doubt you believe?"
		}
	},
	{
		"i am",
		{
			"In what way are you #~#",
			"Do you want to be #~# ?",
		    "Is it because you are #~# that you came to me ?",
		    "How long have you been #~# ?",
		    "Do you believe it is normal to be #~# ?",
		    "Do you enjoy being #~# ?"
			
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
			"Perhaps I am #~# in your fantasies",
			"Do you sometimes think I am #~# ?"
		}
	},
	{
		"you are",
		{
			"What makes you think I am #~# ?",
		    "Does it please you to believe I am #~# ?",
		    "Do you sometimes wish you were #~# ?",
		    "Perhaps you would like to be #~# ."
		}
	},
	{
		"because",
		{
			"Is that the real reason?",
			"What other reasons might there be?",
			"Does that reason seem to explain anything else?",
			"Don't any other reasons come to mind ?"
				
		}
	},
	{
		"why don't you",
		{
		      "Do you believe I don't #~# ?",
		      "Perhaps I will #~# in good time.",
		      "Should you #~# yourself ?",
		      "You want me to #~# ?"
				
		}
	},


	{
		"why can't i",
		{
		    "Do you think you should be able to #~# ?",
		    "Do you want to be able to #~# ?",
		    "Do you believe this will help you to #~# ?",
		    "Have you any idea why you can't #~# ?"
				
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
			"What if you could #~# ?",
			"How do you think that you can't #~# ?",
		    "Have you tried ?",
		    "Perhaps you could #~# now.",
		    "Do you really want to be able to #~# ?"
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
		"i don't",
		{
		    "Don't you really #~# ?",
		    "Why don't you #~# ?",
		    "Do you wish to be able to #~# ?",
		    "Does that trouble you ?"
		}
	},
	
	{
		"do i feel",
		{
		    "Tell me more about such feelings.",
		    "Do you often feel #~# ?",
		    "Do you enjoy feeling #~# ?",
		    "Of what does feeling #~# remind you ?"
		}
	},
	
	
	{
		"i * you",
		{
			"Perhaps in your fantasy we >|< each other",
			"Do you wish to >|< me ?",
		    "You seem to need to >|< me.",
		    "Do you >|< anyone else ?"
				
				
				
		}
	},
	{
		"you * me",
		{
		    "Why do you think I >|< you ?",
		    "You like to think I >|< you -- don't you ?",
		    "What makes you think I >|< you ?",
		    "Really, I >|< you ?",
		    "Do you wish to believe I >|< you ?",
		    "Suppose I did >|< you -- what would that mean ?",
		    "Does someone else believe I >|< you ?"
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
			"I understand",
			"I see."
		}
	},
	{
		"i",
		{
		    "You say #~# ?",
		    "Can you elaborate on that ?",
		    "Do you say #~# for some special reason ?",
		    "That's quite interesting."
		}
	},
	{
		"no",
		{
			"Why not?",
			"You are being a bit negative",
			"Are you saying \"NO\" just to be negative?",
			    "Why \"NO\" ?"
				
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
			"You are thinking of a special person",
			"Realy, #~# ?",
		    "Surely not #~# .",
		    "Who, may I ask ?",
		    "Someone special perhaps ?",
		    "You have a particular person in mind, don't you ?",
		    "Who do you think you're talking about ?"
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
			"What comes to your mind when you ask that?",
		    "Are such questions much on your mind ?",
		    "What answer would please you most ?",
		    "Have you asked such questions before ?",
		    "Have you asked anyone else ?"				
		}
	},
	{
		"perhaps",
		{
			"You do not seem quite certain",
			"Why the uncertain tone ?",
			"Can't you be more positive ?",
			 "You aren't sure ?",
			 "Don't you know ?",
		}
	},
	{
		"are",
		{
			"Did you think they might not be #~#",
			"Possibly they are #~#",
		    "Why are you interested in whether I am #~# or not ?",
		    "Would you prefer if I weren't #~# ?",
		    "Perhaps I am #~# in your fantasies.",
		    "Do you sometimes think I am #~# ?"
				
				
		}
	},
	{
		"your",
		{
		    "Why are you concerned over my #~# ?",
		    "What about your own #~# ?",
		    "Are you worried about someone else's #~# ?",
		    "Really, my #~# ?"
		}
	},
	{
		"my",
		{
		    "Lets discuss further why your #~# .",
		    "Earlier you said your #~# .",
		    "But your #~# .",
		    "Does that have anything to do with the fact that your #~# ?",
		    "Tell me more about your family.",
		    "Who else in your family #~# ?",
		    "Your #~# ?",
		    "What else comes to mind when you think of your #~# ?",
		    "Why do you say your #~# ?",
		    "Does that suggest anything else which belongs to you ?",
		    "Is it important that your #~# ?"
		}
	},
	{
		"you",
		{
		    "We were discussing you -- not me.",
		    "Oh, I #~# ?",
		    "You're not really talking about me -- are you ?",
		    "What are your feelings now ?"
		}
	},

	{
		"can you",
		{
	        "You believe I can #~# don't you ?"
		}
	},

	{
		"can i",
		{
	        "Whether or not you can #~# depends on you more than me.",
	        "Do you want to be able to #~# ?",
	        "Perhaps you don't want to #~# ."
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
			"Do you feel strongly about discussing such things?",
			"You say  ?",
		    "Can you elaborate on that ?",
		    "Do you say that for some special reason ?",
		    "That's quite interesting."
				
				
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
