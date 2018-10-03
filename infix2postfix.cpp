#include "infix2postfix.h"
#include <cassert>

int Evaluate(std::string const & postfix)
{
  const auto stack = new int[postfix.length()];
  unsigned stackwalker = 0;

  for (auto token : postfix)
  {
    //  When we see an operator we:
    //pop the top 2 items(operands)
    //  perform the arithmetic :
    //operand1 operator operand2
    //  push the result of the arithmetic
    if (token == '+')
    {
      --stackwalker;
      stack[stackwalker - 1] = stack[stackwalker - 1] + stack[stackwalker];
    }
    else if (token == '-')
    {
      --stackwalker;
      stack[stackwalker - 1] = stack[stackwalker - 1] - stack[stackwalker];
    }
    else if (token == '*')
    {
      --stackwalker;
      stack[stackwalker - 1] = stack[stackwalker - 1] * stack[stackwalker];
    }
    else if (token == '/')
    {
      --stackwalker;
      stack[stackwalker - 1] = stack[stackwalker - 1] / stack[stackwalker];
    }
    //When we see an operand, we push it on the stack
    else if ('0' <= token && token <= '9')
    {
      stack[stackwalker++] = token - '0';
    }
  }

  // When we have no more tokens, the answer is on the top of the stack
  // (It will be the only item on the stack.)
  auto result = stack[0];
  delete[] stack;

  return result;
}

std::string Infix2postfix(std::string const & postfix)
{
  std::string output;
  std::string stack;

  for (auto token : postfix)
  {
    //Operand - send to the output
    if ('0' <= token && token <= '9')
    {
      output.push_back(token);
    }
    //  Left parenthesis - push onto the stack
    else if (token == '(')
    {
      stack.push_back(token);
    }
    //  Right parenthesis - operators are popped off the stack and sent to
    //  the output until a left parenthesis is found(and then discarded).
    else if (token == ')')
    {
      auto top = stack.back();
      stack.pop_back();
      do
      {
        output.push_back(top);
        top = stack.back();
        stack.pop_back();
      }
      while (top != '(');
    }
    //  Operator
    else if (token == '+' || token == '-' || token == '*' || token == '/')
    {
      //  If the stack is empty, push the operator.
      if (stack.empty())
      {
        stack.push_back(token);
      }
      //  If the top of the stack is a left parenthesis, push the operator
      //  onto the stack.
      else if (stack.back() == '(')
      {
        stack.push_back(token);
      }
      //  If the top of the stack is an operator which has the same or lower
      //  precendence than the scanned operator, push the scanned operator.
      else if (token == '*' || token == '/' || stack.back() == '+'
        || stack.back() == '-'
        )
      {
        stack.push_back(token);
      }
      //  If the top of the stack is an operator which has higher precedence,
      //  pop the stack and send to the output.Repeat the algorithm
      //  with the new top of stack.
      else if ((token == '+' || token == '-')
        && (stack.back() == '*'|| stack.back() == '/')
        )
      {
        const auto top = stack.back();
        stack.pop_back();
        output.push_back(top);
        stack.push_back(token);
      }
    }
  }

  //  If the input stream is empty and there are still operators on the stack,
  //  pop all of them and add them to the output.
  while (!stack.empty())
  {
    const auto top = stack.back();
    stack.pop_back();
    output.push_back(top);
  }

  return output;
}
