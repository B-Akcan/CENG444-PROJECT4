using namespace std;
#include <bits/stdc++.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "dgevalsup.h"

#include "dgevalsyn.tab.hh"
#include "MyParser.h"
#include "FlexLexer.h"
#include "MyFlexLexer.h"
#include "MyParser.h"


void DGEval::scanConstantFolding() {
   for (DGEvalStatementNode *statement = draftedStatements->head; statement != nullptr; statement = statement->next) {
      if (statement->exp != nullptr) {
         scanConstantFolding(nullptr, statement->exp);
      }
   }
}

void DGEval::scanConstantFolding(DGEvalExpNode *parentNode, DGEvalExpNode *node) {
   if (node == nullptr) {
      return;
   }
   
   node->stackLoad = 1; 

   if (node->opCode == CONST) {
      return;
   }
   
   if (node->left != nullptr) {
      scanConstantFolding(node, node->left);
   }
   if (node->right != nullptr) {
      scanConstantFolding(node, node->right);
   }
   
   bool transformed = false;
   
   switch (node->opCode) {
      case OP_ADD: {
         if (node->left != nullptr && node->right != nullptr) {
            if (node->left->type.dim == 0 && node->right->type.dim == 0) {
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
                     node->type.type = DGEvalType::DGString;
                     node->idNdx = 3;
                     node->stringValue = new string(*(node->left->stringValue) + *(node->right->stringValue));
                     delete node->left;
                     delete node->right;
                     node->left = node->right = nullptr;
                     transformed = true;
                  } else { // s1 + s2
                     node->type.type = DGEvalType::DGString;
                     node->idNdx = 4;
                  }
               } else if (node->left->type.type == DGEvalType::DGString && node->right->type.type == DGEvalType::DGNumber) { // string + number concatenation
                  if (node->left->opCode == CONST && node->right->opCode == CONST) { // "a" + 5
                     node->type.type = DGEvalType::DGString;
                     node->idNdx = 4;
                     *(node->stringValue) = *(node->left->stringValue) + to_string(node->right->doubleValue);
                     delete node->left;
                     delete node->right;
                     node->left = node->right = nullptr;
                     transformed = true;
                  } else { // s + a
                     node->type.type = DGEvalType::DGString;
                     node->idNdx = 4;
                  }
               } else if (node->left->type.type == DGEvalType::DGNumber && node->right->type.type == DGEvalType::DGString) { // number + string concatenation
                  if (node->left->opCode == CONST && node->right->opCode == CONST) { // 5 + "a"
                     node->type.type = DGEvalType::DGString;
                     node->idNdx = 4;
                     *(node->stringValue) = to_string(node->left->doubleValue) + *(node->right->stringValue);
                     delete node->left;
                     delete node->right;
                     node->left = node->right = nullptr;
                     transformed = true;
                  } else { // a + s
                     node->type.type = DGEvalType::DGString;
                     node->idNdx = 4;
                  }
               } else if (node->left->type.type == DGEvalType::DGString && *(node->left->stringValue) == "") { // "" + x 
                  delete node->left;
                  node->left = nullptr;
                  node = node->right;
                  node->type.type = DGEvalType::DGString;
                  node->idNdx = 4;
                  transformed = true;
               } else if (node->right->type.type == DGEvalType::DGString && *(node->right->stringValue) == "") { // x + ""
                  delete node->right;
                  node->right = nullptr;
                  node = node->left;
                  node->type.type = DGEvalType::DGString;
                  node->idNdx = 4;
                  transformed = true;
               }
            } else if (node->left->type.dim > 0 && node->right->type.dim == node->left->type.dim - 1 && node->left->type.type == node->right->type.type) { // array push
               node->opCode = LRT;
               node->idNdx = 2;
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
         } else if (node->left != nullptr && node->right != nullptr &&
                    node->left->type.type == DGEvalType::DGString && node->right->type.type == DGEvalType::DGString &&
                    node->left->type.dim == 0 && node->right->type.dim == 0) {
            // String comparison LRT transformation
            node->opCode = LRT;
            node->idNdx = 6; // strcmp function
            node->type.type = DGEvalType::DGBoolean;
            node->type.dim = 0;
            switch (node->opCode) {
               case OP_EQ: node->doubleValue = 0; break;
               case OP_NEQ: node->doubleValue = 1; break;
               case OP_GT: node->doubleValue = 2; break;
               case OP_LT: node->doubleValue = 3; break;
               case OP_GTE: node->doubleValue = 4; break;
               case OP_LTE: node->doubleValue = 5; break;
            }
            transformed = true;
            node->stackLoad = 1;
         } else if (node->left != nullptr && node->right != nullptr &&
                    node->left->type == node->right->type &&
                    node->left->type.dim > 0 && // Must be an array
                    (node->opCode == OP_EQ || node->opCode == OP_NEQ)) {
            // Array comparison LRT transformation
            node->opCode = LRT;
            node->idNdx = 7; // arrcmp function
            node->type.type = DGEvalType::DGBoolean;
            node->type.dim = 0;
            switch (node->opCode) {
               case OP_EQ: node->doubleValue = 0; break;
               case OP_NEQ: node->doubleValue = 1; break;
            }
            transformed = true;
            node->stackLoad = 1;
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
         bool isTopmostComma = false;
         if (parentNode == nullptr || parentNode->opCode != OP_COMMA) {
            isTopmostComma = true;
         }

         bool inCallOrArrayLiteral = false;
         if (parentNode != nullptr) {
            if (parentNode->opCode == OP_CALL && parentNode->right == node) {
               inCallOrArrayLiteral = true;
            } else if (parentNode->opCode == LRT && parentNode->left == node) {
               inCallOrArrayLiteral = true;
            }
         }

         node->stackLoad = 0;
         
         if (node->right != nullptr) {
            if ((optimization & OPTIMIZE_DC_EXPPART) && isTopmostComma && !inCallOrArrayLiteral) {
               if (node->right->functionCallCount == 0 && node->right->assignmentCount == 0) {
                  node->right->eliminateIC = true;
               } else {
                  node->right->eliminateIC = false;
               }
            } else {
               node->right->eliminateIC = false;
            }
            
            if (!node->right->eliminateIC) {
               node->stackLoad += node->right->stackLoad;
            }
         }

         DGEvalExpNode* currentLeft = node->left;
         while (currentLeft != nullptr) {
            if (currentLeft->opCode == OP_COMMA) {
               if (currentLeft->right != nullptr) {
                  if ((optimization & OPTIMIZE_DC_EXPPART) && isTopmostComma && !inCallOrArrayLiteral) {
                     if (currentLeft->right->functionCallCount == 0 &&
                        currentLeft->right->assignmentCount == 0) {
                        currentLeft->right->eliminateIC = true;
                     } else {
                        currentLeft->right->eliminateIC = false;
                     }
                  } else {
                     currentLeft->right->eliminateIC = false;
                  }
                  if (!currentLeft->right->eliminateIC) {
                     node->stackLoad += currentLeft->right->stackLoad;
                  }
               }
               currentLeft = currentLeft->left;
            } else { // currentLeft is the left-most operand of all commas, thus should not be eliminated
               currentLeft->eliminateIC = false;
               node->stackLoad += currentLeft->stackLoad;
               break;
            }
         }

         if (node->stackLoad == 0 && node->opCode == OP_COMMA) {
             node->stackLoad = 1;
         }
         break;
      }

      case OP_AA: {
         node->opCode = LRT;
         node->idNdx = 1;
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
      if (statement->exp != nullptr) {
         bool isEffective = true;
         if (optimization & OPTIMIZE_DC_STATEMENT) {
            isEffective = (statement->asPredecessorCount > 0) || 
                           (statement->exp->functionCallCount > 0) || 
                           (statement->exp->assignmentCount > 0);
         }

         if (isEffective) {
            scanForIC(nullptr, statement->exp);
         }
      }
   }

   ic->emitIC(LRT, 8);
}

void DGEval::scanForIC(DGEvalExpNode *parentNode, DGEvalExpNode *node) {
   if (node == nullptr) {
      return;
   }
   
   if (node->eliminateIC) {
      return;
   }
   
   if (node->opCode == OP_COND) {
      scanForIC(node, node->left);
      DGEvalICInst *jfInst = ic->emitIC(JF, 0, node->right->left->type);
      int jfIndex = ic->instCount() - 1;
      scanForIC(node, node->right->left);
      DGEvalICInst *jmpInst = ic->emitIC(JMP, 0, node->right->right->type);
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

   if (node->right != nullptr) {
      scanForIC(node, node->right);
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
               ic->emitIC(LRT, 3, node->type)->strConstant = node->stringValue;
               break;
         }
         break;
      }
      
      case OP_ADD: {
         if (node->type.type == DGEvalType::DGString && node->type.dim == 0) { // string concatenation
            ic->emitIC(LRT, node->idNdx, node->type)->strConstant = node->stringValue;
         } else {
            ic->emitIC(node->opCode, 0, node->type);
         }
         break;
      }

      case OP_SUB:
      case OP_MUL:
      case OP_DIV:
      case OP_MINUS:
      case OP_NOT:
      case OP_BAND:
      case OP_BOR:
      case OP_NOP: {
         ic->emitIC(node->opCode, 0, node->type);
         break;
      }

      case OP_EQ:
      case OP_NEQ:
      case OP_LT:
      case OP_LTE:
      case OP_GT:
      case OP_GTE:
      {
         ic->emitIC(node->opCode, 0, node->left->type);
         break;
      }
      
      case OP_ASSIGN: {
         ic->emitIC(OP_ASSIGN, node->idNdx, node->type)->strConstant = &(symbolTable.symbolAt(node->idNdx)->name);
         break;
      }
      
      case OP_CALL: {
         ic->emitIC(OP_CALL, node->pVector != nullptr ? (int)node->pVector->size() : 0, node->type)->numConstant = node->left->idNdx;
         break;
      }
      
      case INSID: {
         if (node->idNdx >= 0 &&
               !(parentNode != nullptr && parentNode->left == node && parentNode->opCode == OP_CALL)) {
            ic->emitIC(INSID, node->idNdx, node->type)->strConstant = node->stringValue;
         }
         break;
      }

      case LRT: {
         ic->emitIC(LRT, node->idNdx, node->type)->numConstant = (node->pVector != nullptr ? (int)node->pVector->size() : 0);
         break;
      }

      case OP_COMMA: {
         if (node->stackLoad > 1 && parentNode != nullptr && parentNode->opCode != OP_COMMA && parentNode->opCode != OP_CALL && parentNode->opCode != LRT) {
            ic->emitIC(POP, node->stackLoad - 1, { DGEvalType::DGNone, 0 });
         }
         break;
      }
   }

   if (parentNode == nullptr) {
      ic->emitIC(POP, node->stackLoad, { DGEvalType::DGNone, 0 });
   }
}

void DGEval::peepholeIC() {
   if (!(optimization & (OPTIMIZE_PH_OFFLOAD | OPTIMIZE_PH_CONSTSINK))) {
      return;
   }

   for (int i = 1; i < ic->instCount(); ++i) {
      DGEvalICInst *inst1 = ic->instructionAt(i - 1);
      DGEvalICInst *inst2 = ic->instructionAt(i);

      if ((optimization & OPTIMIZE_PH_CONSTSINK) && 
            inst1 != nullptr && inst2 != nullptr && inst2->opCode == POP) {
         if ((inst1->opCode == CONST) || // numeric or boolean constant
               (inst1->opCode == LRT && inst1->p1 == 3)) { // string constant
            ic->markRemoval(i - 1, 2);
         }
      }
   }

   if (optimization & OPTIMIZE_PH_OFFLOAD) {
      for (int i = 2; i < ic->instCount(); ++i) {
         DGEvalCodePathWindow window;
         window.build(ic, i, 3);
         bool can_apply_offload_here = true;

         if (window.path.empty()) {
             continue;
         }

         for (DGEvalCodePath *path : window.path) {
            DGEvalICInst *inst2 = ic->instructionAt(i); 
            DGEvalICInst *inst1 = ic->instructionAt(i - 1);
            DGEvalICInst *inst0 = ic->instructionAt(i - 2);

            if (!(inst0 != nullptr && inst1 != nullptr && inst2 != nullptr &&
                  inst0->opCode == OP_ASSIGN && inst1->opCode == POP && inst2->opCode == INSID &&
                  inst0->strConstant != nullptr && inst2->strConstant != nullptr &&
                  *(inst0->strConstant) == *(inst2->strConstant))) {
               can_apply_offload_here = false;
            }
         }

         if (can_apply_offload_here) {
            ic->markRemoval(i - 1, 2);
         }
      }
   }

   ic->applyRemoval();
}

double DGEval::mean(DGEvalArrayDouble *array) {
   if (array == nullptr || array->arr->empty()) {
      return 0.0;
   }
   
   double sum = 0.0;
   for (double val : *(array->arr)) {
      sum += val;
   }
   return sum / array->arr->size();
}

double DGEval::stddev(DGEvalArrayDouble *array) {
   if (array == nullptr || array->arr->size() < 2) {
      return 0.0;
   }
   
   double avg = mean(array);
   double sumSquares = 0.0;
   
   for (double val : *(array->arr)) {
      double diff = val - avg;
      sumSquares += diff * diff;
   }
   
   return sqrt(sumSquares / (array->arr->size() - 1));
}

double DGEval::count(DGEvalArrayDouble *array) {
   if (array == nullptr) {
      return 0.0;
   }
   return (double)array->arr->size();
}

double DGEval::min(DGEvalArrayDouble *array) {
   if (array == nullptr || array->arr->empty()) {
      return 0.0;
   }
   
   double minVal = array->arr->at(0);
   for (double val : *(array->arr)) {
      if (val < minVal) {
         minVal = val;
      }
   }
   return minVal;
}

double DGEval::max(DGEvalArrayDouble *array) {
   if (array == nullptr || array->arr->empty()) {
      return 0.0;
   }
   
   double maxVal = array->arr->at(0);
   for (double val : *(array->arr)) {
      if (val > maxVal) {
         maxVal = val;
      }
   }
   return maxVal;
}

double DGEval::sin(double number) {
   return std::sin(number);
}

double DGEval::cos(double number) {
   return std::cos(number);
}

double DGEval::tan(double number) {
   return std::tan(number);
}

double DGEval::pi() {
   return M_PI;
}

double DGEval::atan(double number) {
   return std::atan(number);
}

double DGEval::asin(double number) {
   return std::asin(number);
}

double DGEval::acos(double number) {
   return std::acos(number);
}

double DGEval::exp(double number) {
   return std::exp(number);
}

double DGEval::ln(double number) {
   return std::log(number);
}

double DGEval::print(string *str) {
   if (str != nullptr) {
      cout << *str;
   }
   return 0.0;
}

double DGEval::random(double number) {
   if (number <= 0) {
      return 0.0;
   }
   return (double)rand() / RAND_MAX * number;
}

double DGEval::len(string *str) {
   if (str == nullptr) {
      return 0.0;
   }
   return (double)str->length();
}

string *DGEval::right(DGEval *dgEval, string *str, double n) {
   if (str == nullptr || n <= 0) {
      return new string("");
   }
   
   int len = (int)n;
   if (len >= str->length()) {
      return new string(*str);
   }
   
   return new string(str->substr(str->length() - len));
}

string *DGEval::left(DGEval *dgEval, string *str, double n) {
   if (str == nullptr || n <= 0) {
      return new string("");
   }
   
   int len = (int)n;
   if (len >= str->length()) {
      return new string(*str);
   }
   
   return new string(str->substr(0, len));
}

DGEvalDynamicFunc *DGEval::generateCode() {
   if (ic == nullptr) {
      return nullptr;
   }

   X64CodeBag *codeBag = new X64CodeBag(this);
   codeBag->emitPrologue(0);
   
   for (int i = 0; i < ic->instCount(); i++) {
      DGEvalICInst *inst = ic->instructionAt(i);
      if (inst != nullptr) {
         codeBag->translateInstruction(inst);
      }
   }
   
   codeBag->emitEpilogue();
   DGEvalDynamicFunc *func = (DGEvalDynamicFunc *)codeBag->createCodeBase();
   delete codeBag;
   return func;
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
