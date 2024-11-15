#include <cstring>
#include <iostream>

#include "args.h"
#include "chat.h"
#include "chatter.h"

using namespace std;

// TODO: Implementation des signaux (je sais pas ou le mettre)

int main(int argc, char* argv[]) {
  Args arg = parseArgs(argc, argv);
  Chatter usr1(arg.SENDER_NAME);
  cout << " - " << usr1.name() << endl;
  Chatter usr2(arg.RECEIVER_NAME);
  cout << " - " << usr2.name() << endl;
  Chat chat(usr1, usr2);
  return 0;
}
