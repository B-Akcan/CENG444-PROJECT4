using namespace std;
#include <bits/stdc++.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include "dgevalsup.h"

#include "dgevalsyn.tab.hh"
#include "MyParser.h"
#include "FlexLexer.h"
#include "MyFlexLexer.h"
#include "MyParser.h"


void DGEval::scanConstantFolding() {
   for (DGEvalStatementNode *statement = statements->head; statement != nullptr; statement = statement->next) {
      if (statement->exp != nullptr) {
         scanConstantFolding(nullptr, statement->exp);
      }
   }
}

void DGEval::scanConstantFolding(DGEvalExpNode *parentNode, DGEvalExpNode *node) {
   if (node == nullptr) {
      return;
   }
   
   // Default stackLoad to 1 for most expressions (they push one result onto the stack)
   node->stackLoad = 1; 

   if (node->opCode == CONST) {
      return; // Already set stackLoad to 1
   }
   
   // Recursively process children first
   if (node->left != nullptr) {
      scanConstantFolding(node, node->left);
   }
   if (node->right != nullptr) {
      scanConstantFolding(node, node->right);
   }
   
   bool transformed = false;
   
   switch (node->opCode) {
      case OP_ADD: {
         if (node->left != nullptr && node->right != nullptr && 
               node->left->type.dim == 0 && node->right->type.dim == 0) {
            if (node->left->type.type == DGEvalType::DGNumber && node->right->type.type == DGEvalType::DGNumber) { // add two numbers
               if (node->left->opCode == CONST && node->right->opCode == CONST) { // 5 + 3
                  node->opCode = CONST;
                  node->type.type = DGEvalType::DGNumber;
                  node->doubleValue = node->left->doubleValue + node->right->doubleValue;
                  delete node->left;
                  delete node->right;
                  node->left = node->right = nullptr;
                  transformed = true;
               } else if (node->left->opCode == CONST && node->left->doubleValue == 0) { // 0 + x
                  delete node->left;
                  node->left = nullptr;
                  node = node->right;
                  transformed = true;
               } else if (node->right->opCode == CONST && node->right->doubleValue == 0) { // x + 0
                  delete node->right;
                  node->right = nullptr;
                  node = node->left;
                  transformed = true;
               }
            } else if (node->left->type.type == DGEvalType::DGString && node->right->type.type == DGEvalType::DGString) { // string + string concatenation
               if (node->left->opCode == CONST && node->right->opCode == CONST) { // "a" + "b"
                  node->opCode = CONST;
                  node->type.type = DGEvalType::DGString;
                  *(node->stringValue) = *(node->left->stringValue) + *(node->right->stringValue);
                  delete node->left;
                  delete node->right;
                  node->left = node->right = nullptr;
                  transformed = true;
               }
            } else if (node->left->type.type == DGEvalType::DGString && node->right->type.type == DGEvalType::DGNumber) { // string + number concatenation
               if (node->left->opCode == CONST && node->right->opCode == CONST) { // "a" + 5
                  node->opCode = CONST;
                  node->type.type = DGEvalType::DGString;
                  *(node->stringValue) = *(node->left->stringValue) + to_string(node->right->doubleValue);
                  delete node->left;
                  delete node->right;
                  node->left = node->right = nullptr;
                  transformed = true;
               }
            } else if (node->left->type.type == DGEvalType::DGNumber && node->right->type.type == DGEvalType::DGString) { // number + string concatenation
               if (node->left->opCode == CONST && node->right->opCode == CONST) { // 5 + "a"
                  node->opCode = CONST;
                  node->type.type = DGEvalType::DGString;
                  *(node->stringValue) = to_string(node->left->doubleValue) + *(node->right->stringValue);
                  delete node->left;
                  delete node->right;
                  node->left = node->right = nullptr;
                  transformed = true;
               }
            } else if (node->left->type.type == DGEvalType::DGString && *(node->left->stringValue) == "") { // "" + x 
               delete node->left;
               node->left = nullptr;
               node = node->right;
               transformed = true;
            } else if (node->right->type.type == DGEvalType::DGString && *(node->right->stringValue) == "") { // x + ""
               delete node->right;
               node->right = nullptr;
               node = node->left;
               transformed = true;
            }
         }
         break;
      }

      case OP_SUB: {
         if (node->left != nullptr && node->right != nullptr &&
               node->left->type.type == DGEvalType::DGNumber && node->left->type.dim == 0 &&
               node->right->type.type == DGEvalType::DGNumber && node->right->type.dim == 0) {
            if (node->left->opCode == CONST && node->right->opCode == CONST) { // 5 - 3
               node->opCode = CONST;
               node->type.type = DGEvalType::DGNumber;
               node->doubleValue = node->left->doubleValue - node->right->doubleValue;
               delete node->left;
               delete node->right;
               node->left = node->right = nullptr;
               transformed = true;
            } else if (node->left->opCode == CONST && node->left->doubleValue == 0) { // 0 - x
               delete node->left;
               node->left = nullptr;
               node = node->right;
               node->doubleValue = -node->doubleValue;
               transformed = true;
            } else if (node->right->opCode == CONST && node->right->doubleValue == 0) { // x - 0
               delete node->right;
               node->right = nullptr;
               node = node->left;
               transformed = true;
            }
         }
         break;
      }

      case OP_MUL: {
         if (node->left != nullptr && node->right != nullptr && 
               node->left->type.type == DGEvalType::DGNumber && node->left->type.dim == 0 &&
               node->right->type.type == DGEvalType::DGNumber && node->right->type.dim == 0) {
            if (node->left->opCode == CONST && node->right->opCode == CONST) { // 5 * 3
               node->opCode = CONST;
               node->type.type = DGEvalType::DGNumber;
               node->doubleValue = node->left->doubleValue * node->right->doubleValue;
               delete node->left;
               delete node->right;
               node->left = node->right = nullptr;
               transformed = true;
            } else if (node->left->opCode == CONST && node->left->doubleValue == 0 && 
                        node->right->functionCallCount == 0 && node->right->assignmentCount == 0) { // 0 * x
               node->opCode = CONST;
               node->type.type = DGEvalType::DGNumber;
               node->doubleValue = 0;
               delete node->left;
               delete node->right;
               node->left = node->right = nullptr;
               transformed = true;
            } else if (node->right->opCode == CONST && node->right->doubleValue == 0 &&
                        node->left->functionCallCount == 0 && node->left->assignmentCount == 0) { // x * 0
               node->opCode = CONST;
               node->type.type = DGEvalType::DGNumber;
               node->doubleValue = 0;
               delete node->left;
               delete node->right;
               node->left = node->right = nullptr;
               transformed = true;
            } else if (node->left->opCode == CONST && node->left->doubleValue == 1) { // 1 * x
               delete node->left;
               node->left = nullptr;
               node = node->right;
               transformed = true;
            } else if (node->right->opCode == CONST && node->right->doubleValue == 1) { // x * 1
               delete node->right;
               node->right = nullptr;
               node = node->left;
               transformed = true;
            }
         }
         break;
      }

      case OP_DIV: {
         if (node->left != nullptr && node->right != nullptr && 
               node->left->type.type == DGEvalType::DGNumber && node->left->type.dim == 0 &&
               node->right->type.type == DGEvalType::DGNumber && node->right->type.dim == 0) {
            if (node->right->doubleValue == 0) { // x / 0
               return;
            }

            if (node->left->opCode == CONST && node->right->opCode == CONST) { // 5 / 3
               node->opCode = CONST;
               node->type.type = DGEvalType::DGNumber;
               node->doubleValue = node->left->doubleValue / node->right->doubleValue;
               delete node->left;
               delete node->right;
               node->left = node->right = nullptr;
               transformed = true;
            } else if (node->left->opCode == CONST && node->left->doubleValue == 0 &&
                        node->right->functionCallCount == 0 && node->right->assignmentCount == 0) { // 0 / x
               node->opCode = CONST;
               node->type.type = DGEvalType::DGNumber;
               node->doubleValue = 0;
               delete node->left;
               delete node->right;
               node->left = node->right = nullptr;
               transformed = true;
            } else if (node->right->opCode == CONST && node->right->doubleValue == 1) { // x / 1
               delete node->right;
               node->right = nullptr;
               node = node->left;
               transformed = true;
            }
         }
         break;
      }
      
      case OP_MINUS: {
         if (node->left != nullptr && node->left->opCode == CONST && 
             node->left->type.type == DGEvalType::DGNumber && node->left->type.dim == 0) { // -5
            node->opCode = CONST;
            node->type.type = DGEvalType::DGNumber;
            node->doubleValue = -node->left->doubleValue;
            delete node->left;
            node->left = nullptr;
            transformed = true;
         }
         break;
      }
      
      case OP_NOT: {
         if (node->left != nullptr && node->left->opCode == CONST && 
             node->left->type.type == DGEvalType::DGBoolean && node->left->type.dim == 0) { // !true
            node->opCode = CONST;
            node->type.type = DGEvalType::DGBoolean;
            node->boolValue = !node->left->boolValue;
            delete node->left;
            node->left = nullptr;
            transformed = true;
         }
         break;
      }
      
      case OP_EQ:
      case OP_NEQ:
      case OP_LT:
      case OP_LTE:
      case OP_GT:
      case OP_GTE: {
         if (node->left != nullptr && node->right != nullptr && 
             node->left->opCode == CONST && node->right->opCode == CONST &&
             node->left->type.type == node->right->type.type &&
             node->left->type.dim == 0 && node->right->type.dim == 0) {
            bool result;
            if (node->left->type.type == DGEvalType::DGNumber) {
               switch (node->opCode) {
                  case OP_EQ: result = node->left->doubleValue == node->right->doubleValue; break;
                  case OP_NEQ: result = node->left->doubleValue != node->right->doubleValue; break;
                  case OP_LT: result = node->left->doubleValue < node->right->doubleValue; break;
                  case OP_LTE: result = node->left->doubleValue <= node->right->doubleValue; break;
                  case OP_GT: result = node->left->doubleValue > node->right->doubleValue; break;
                  case OP_GTE: result = node->left->doubleValue >= node->right->doubleValue; break;
               }
            } else if (node->left->type.type == DGEvalType::DGBoolean) {
               switch (node->opCode) {
                  case OP_EQ: result = node->left->boolValue == node->right->boolValue; break;
                  case OP_NEQ: result = node->left->boolValue != node->right->boolValue; break;
               }
            } else if (node->left->type.type == DGEvalType::DGString) {
               switch (node->opCode) {
                  case OP_EQ: result = (node->left->stringValue)->compare(*(node->right->stringValue)) == 0; break;
                  case OP_NEQ: result = (node->left->stringValue)->compare(*(node->right->stringValue)) != 0; break;
                  case OP_LT: result = (node->left->stringValue)->compare(*(node->right->stringValue)) < 0; break;
                  case OP_LTE: result = (node->left->stringValue)->compare(*(node->right->stringValue)) <= 0; break;
                  case OP_GT: result = (node->left->stringValue)->compare(*(node->right->stringValue)) > 0; break;
                  case OP_GTE: result = (node->left->stringValue)->compare(*(node->right->stringValue)) >= 0; break;
               }
            }
            node->opCode = CONST;
            node->type.type = DGEvalType::DGBoolean;
            node->boolValue = result;
            delete node->left;
            delete node->right;
            node->left = node->right = nullptr;
            transformed = true;
         }
         break;
      }
      
      case OP_BAND: {
         if (node->left != nullptr && node->right != nullptr &&
               node->left->type.dim == 0 && node->right->type.dim == 0 &&
               node->left->type.type == DGEvalType::DGBoolean && node->right->type.type == DGEvalType::DGBoolean) {
            if (node->left->opCode == CONST && node->right->opCode == CONST) { // true && false
               node->opCode = CONST;
               node->type.type = DGEvalType::DGBoolean;
               node->boolValue = node->left->boolValue && node->right->boolValue;
               delete node->left;
               delete node->right;
               node->left = node->right = nullptr;
               transformed = true;
            } else if (node->left->opCode == CONST) {
               if (node->left->boolValue == true) { // true && x
                  delete node->left;
                  node->left = nullptr;
                  node = node->right;
                  transformed = true;
               } else { // false && x
                  node->opCode = CONST;
                  node->type.type = DGEvalType::DGBoolean;
                  node->boolValue = false;
                  delete node->left;
                  delete node->right;
                  node->left = node->right = nullptr;
                  transformed = true;
               }
            } else if (node->right->opCode == CONST) {
               if (node->right->boolValue == true) { // x && true
                  delete node->right;
                  node->right = nullptr;
                  node = node->left;
                  transformed = true;
               } else { // x && false
                  node->opCode = CONST;
                  node->type.type = DGEvalType::DGBoolean;
                  node->boolValue = false;
                  delete node->left;
                  delete node->right;
                  node->left = node->right = nullptr;
                  transformed = true;
               }
            }
         }
         break;
      }

      case OP_BOR: {
         if (node->left != nullptr && node->right != nullptr &&
               node->left->type.dim == 0 && node->right->type.dim == 0 &&
               node->left->type.type == DGEvalType::DGBoolean && node->right->type.type == DGEvalType::DGBoolean) {
            if (node->left->opCode == CONST && node->right->opCode == CONST) { // true || false
               node->opCode = CONST;
               node->type.type = DGEvalType::DGBoolean;
               node->boolValue = node->left->boolValue || node->right->boolValue;
               delete node->left;
               delete node->right;
               node->left = node->right = nullptr;
               transformed = true;
            } else if (node->left->opCode == CONST) {
               if (node->left->boolValue == true) { // true || x
                  node->opCode = CONST;
                  node->type.type = DGEvalType::DGBoolean;
                  node->boolValue = true;
                  delete node->left;
                  delete node->right;
                  node->left = node->right = nullptr;
                  transformed = true;
               } else { // false || x
                  delete node->left;
                  node->left = nullptr;
                  node = node->right;
                  transformed = true;
               }
            } else if (node->right->opCode == CONST) {
               if (node->right->boolValue == true) { // x || true
                  node->opCode = CONST;
                  node->type.type = DGEvalType::DGBoolean;
                  node->boolValue = true;
                  delete node->left;
                  delete node->right;
                  node->left = node->right = nullptr;
                  transformed = true;
               } else { // x || false
                  delete node->right;
                  node->right = nullptr;
                  node = node->left;
                  transformed = true;
               }
            }
         }
         break;
      }
      
      case OP_COND: {
         if (node->left != nullptr && node->left->opCode == CONST && 
             node->left->type.type == DGEvalType::DGBoolean && node->left->type.dim == 0 &&
             node->right != nullptr && node->right->left != nullptr && node->right->right != nullptr) {
            if (node->left->boolValue == true) { // true ? x : y
               delete node->left;
               node->left = nullptr;
               delete node->right->right;
               node->right->right = nullptr;
               node = node->right->left;
               node->right->left = nullptr;
               delete node->right;
               node->right = nullptr;
               transformed = true;
            } else { // false ? x : y
               delete node->left;
               node->left = nullptr;
               delete node->right->left;
               node->right->left = nullptr;
               node = node->right->right;
               node->right->right = nullptr;
               delete node->right;
               node->right = nullptr;
               transformed = true;
            }
         }
         break;
      }

      case OP_COMMA: {
         // Determine if it's a topmost comma for optimization purposes
         bool isTopmostComma = false;
         if (parentNode == nullptr || // Root of the statement expression
             (parentNode->opCode != OP_COMMA && parentNode->left == node)) { // Left child and parent is not comma
            isTopmostComma = true;
         }

         // If this comma is the right child of a function call or an array literal, then its parts are effective
         bool inCallOrArrayLiteral = false;
         if (parentNode != nullptr && (parentNode->opCode == OP_CALL || parentNode->opCode == LRT)) {
             if (parentNode->right == node) {
                 inCallOrArrayLiteral = true;
             }
         }

         node->stackLoad = 0; // Reset stackLoad for comma, calculate based on effective children
         
         // Apply OPTIMIZE_DC_EXPPART optimization and calculate stackLoad
         // Process right child (an expression part)
         if (node->right != nullptr) {
            if ((optimization & OPTIMIZE_DC_EXPPART) && isTopmostComma && !inCallOrArrayLiteral) {
               if (node->right->functionCallCount == 0 && node->right->assignmentCount == 0) {
                  node->right->eliminateIC = true;
               }
            }
            if (!node->right->eliminateIC) {
                node->stackLoad += node->right->stackLoad;
            }
         }

         // Traverse the left chain of commas to sum up stackLoads of effective parts
         DGEvalExpNode* currentLeft = node->left;
         while (currentLeft != nullptr) {
             if (currentLeft->opCode == OP_COMMA) {
                 if ((optimization & OPTIMIZE_DC_EXPPART) && isTopmostComma && !inCallOrArrayLiteral) {
                     if (currentLeft->right != nullptr && 
                         currentLeft->right->functionCallCount == 0 && 
                         currentLeft->right->assignmentCount == 0) {
                         currentLeft->right->eliminateIC = true;
                     }
                 }
                 if (currentLeft->right != nullptr && !currentLeft->right->eliminateIC) {
                     node->stackLoad += currentLeft->right->stackLoad;
                 }
                 currentLeft = currentLeft->left; // Move to the next left part in the chain
             } else { // It's a single expression part at the end of the comma chain
                 if ((optimization & OPTIMIZE_DC_EXPPART) && isTopmostComma && !inCallOrArrayLiteral) {
                     if (currentLeft->functionCallCount == 0 && 
                         currentLeft->assignmentCount == 0) {
                         currentLeft->eliminateIC = true;
                     }
                 }
                 if (!currentLeft->eliminateIC) {
                     node->stackLoad += currentLeft->stackLoad;
                 }
                 break; // End of the comma chain
             }
         }
         break;
      }
   }
   
   if (transformed && parentNode != nullptr) {
      if (parentNode->left == node) {
         parentNode->left = node;
      } else if (parentNode->right == node) {
         parentNode->right = node;
      }
   }
}

void DGEval::scanForIC() {
   ic = new DGEvalIC();

   for (DGEvalStatementNode *statement = draftedStatements->head; statement != nullptr; statement = statement->next) {
      bool isEffective = true;
      if (optimization & OPTIMIZE_DC_STATEMENT) {
         isEffective = statement->asPredecessorCount > 0 || 
                      (statement->exp != nullptr && 
                       (statement->exp->functionCallCount > 0 || 
                        statement->exp->assignmentCount > 0));
      }

      if (isEffective && statement->exp != nullptr) {
         scanForIC(nullptr, statement->exp);
      }
   }

   ic->emitIC(LRT, 8);
}

void DGEval::scanForIC(DGEvalExpNode *parentNode, DGEvalExpNode *node) {
   if (node == nullptr) {
      return;
   }
   
   // Skip generating IC for eliminated nodes
   if (node->eliminateIC) {
      return;
   }
   
   if (node->opCode == OP_COND) {
      scanForIC(node, node->left);
      DGEvalICInst *jfInst = ic->emitIC(JF, 0);
      int jfIndex = ic->instCount() - 1;
      
      scanForIC(node, node->right->left);
      DGEvalICInst *jmpInst = ic->emitIC(JMP, 0);
      int jmpIndex = ic->instCount() - 1;
      jfInst->p1 = ic->instCount();
      
      scanForIC(node, node->right->right);
      jmpInst->p1 = ic->instCount();
      
      return;
   }
   
   if (node->opCode == OP_ALT) {
      scanForIC(node, node->left);
      scanForIC(node, node->right);
      
      return;
   }
   
   if (node->left != nullptr) {
      scanForIC(node, node->left);
   }
   
   switch (node->opCode) {
      case CONST: {
         switch (node->type.type) {
            case DGEvalType::DGNumber:
               ic->emitIC(CONST, 0, node->type)->numConstant = node->doubleValue;
               break;
            case DGEvalType::DGBoolean:
               ic->emitIC(CONST, 0, node->type)->boolConstant = node->boolValue;
               break;
            case DGEvalType::DGString:
               ic->emitIC(CONST, 0, node->type)->strConstant = node->stringValue;
               break;
         }
         break;
      }
      
      case OP_ADD:
      case OP_SUB:
      case OP_MUL:
      case OP_DIV:
      case OP_MINUS:
      case OP_NOT:
      case OP_BAND:
      case OP_BOR:
      case OP_EQ:
      case OP_NEQ:
      case OP_LT:
      case OP_LTE:
      case OP_GT:
      case OP_GTE: {
         ic->emitIC(node->opCode, 0, node->type);
         break;
      }
      
      case OP_CALL: {
         ic->emitIC(OP_CALL, node->idNdx, node->type);
         break;
      }
      
      case INSID: {
         ic->emitIC(OP_ASSIGN, 0, node->type)->strConstant = node->stringValue;
         break;
      }

      case OP_COMMA: {
         // The actual instruction for comma is POP with its stackLoad.
         // We need to pop all but the last value from the stack.
         if (node->stackLoad > 1) {
            ic->emitIC(POP, node->stackLoad - 1, { DGEvalType::DGNone, 0 });
         }
         break;
      }
   }
   
   if (node->right != nullptr) {
      scanForIC(node, node->right);
   }
}

void DGEval::peepholeIC() {
   return;
}

double DGEval::mean(DGEvalArrayDouble *array) {
   return 0;
}

double DGEval::stddev(DGEvalArrayDouble *array) {
   return 0;
}

double DGEval::count(DGEvalArrayDouble *array) {
   return 0;
}

double DGEval::min(DGEvalArrayDouble *array) {
   return 0;
}

double DGEval::max(DGEvalArrayDouble *array) {
   return 0;
}

double DGEval::sin(double number) {
   return 0;
}

double DGEval::cos(double number) {
   return 0;
}

double DGEval::tan(double number) {
   return 0;
}

double DGEval::pi() {
   return 0;
}

double DGEval::atan(double number) {
   return 0;
}

double DGEval::asin(double number) {
   return 0;
}

double DGEval::acos(double number) {
   return 0;
}

double DGEval::exp(double number) {
   return 0;
}

double DGEval::ln(double number) {
   return 0;
}

double DGEval::print(string *str) {
   return 0;
}

double DGEval::random(double number) {
   return 0;
}

double DGEval::len(string *str) {
   return 0;
}

string *DGEval::right(DGEval *dgEval, string *str, double n) {
   return nullptr;
}

string *DGEval::left(DGEval *dgEval, string *str, double n) {
   return nullptr;
}

DGEvalDynamicFunc *DGEval::generateCode() {
   return nullptr;
}

void DGEvalIC::writeAsJSON(ostream *outStream) {
   (*outStream) << "[";

   for (int i=0; i<code.size(); i++) {
      code.at(i).inst.writeAsJSON(outStream);

      if (i < code.size() - 1) {
         (*outStream) << ", ";
      }
   }

   (*outStream) << "]";
}

void DGEvalICInst::writeAsJSON(ostream *outStream) {
   (*outStream) << "{";

   (*outStream) << "\"mnemonic\": \"" << DGEval::opStr[opCode]
                << "\", \"opCode\": " << opCode
                << ", \"p1\": " << p1
                << ", \"type\": " << (int)type.type
                << ", \"dim\": " << type.dim;
                
   if (opCode==CONST && type.dim==0)
   {
      switch (type.type)
      {
         case DGEvalType::DGString:
            (*outStream) << ", \"value\": ";
            DGEval::writeStrValue(outStream, strConstant);
            break;
         case DGEvalType::DGNumber:
            (*outStream) << ", \"value\": " << numConstant;
            break;
         case DGEvalType::DGBoolean:
            (*outStream) << ", \"value\": " << boolConstant;
            break;
         default:
            break;
      }
   }

   (*outStream) << "}";
}
