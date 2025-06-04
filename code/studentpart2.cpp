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
      case OP_ADD:
      case OP_SUB:
      case OP_MUL:
      case OP_DIV: {
         if (node->left != nullptr && node->right != nullptr && 
             node->left->opCode == CONST && node->right->opCode == CONST &&
             node->left->type.type == DGEvalType::DGNumber && node->left->type.dim == 0 &&
             node->right->type.type == DGEvalType::DGNumber && node->right->type.dim == 0) {
            double result;
            switch (node->opCode) {
               case OP_ADD: result = node->left->doubleValue + node->right->doubleValue; break;
               case OP_SUB: result = node->left->doubleValue - node->right->doubleValue; break;
               case OP_MUL: result = node->left->doubleValue * node->right->doubleValue; break;
               case OP_DIV: 
                  if (node->right->doubleValue == 0) {
                     return;
                  }
                  result = node->left->doubleValue / node->right->doubleValue; 
                  break;
            }
            node->opCode = CONST;
            node->type.type = DGEvalType::DGNumber;
            node->doubleValue = result;
            delete node->left;
            delete node->right;
            node->left = node->right = nullptr;
            transformed = true;
         }
         break;
      }
      
      case OP_MINUS: {
         if (node->left != nullptr && node->left->opCode == CONST && 
             node->left->type.type == DGEvalType::DGNumber && node->left->type.dim == 0) {
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
             node->left->type.type == DGEvalType::DGBoolean && node->left->type.dim == 0) {
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
      
      case OP_BAND:
      case OP_BOR: {
         if (node->left != nullptr && node->right != nullptr && 
             node->left->opCode == CONST && node->right->opCode == CONST &&
             node->left->type.type == DGEvalType::DGBoolean && node->right->type.type == DGEvalType::DGBoolean &&
             node->left->type.dim == 0 && node->right->type.dim == 0) {
            bool result;
            switch (node->opCode) {
               case OP_BAND: result = node->left->boolValue && node->right->boolValue; break;
               case OP_BOR: result = node->left->boolValue || node->right->boolValue; break;
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
      
      case OP_COND: {
         if (node->left != nullptr && node->left->opCode == CONST && 
             node->left->type.type == DGEvalType::DGBoolean && node->left->type.dim == 0 &&
             node->right != nullptr && node->right->left != nullptr && node->right->right != nullptr) {
            DGEvalExpNode *selected = node->left->boolValue ? node->right->left : node->right->right;
            if (selected != nullptr && selected->opCode == CONST) {
               node->opCode = CONST;
               node->type = selected->type;
               switch (selected->type.type) {
                  case DGEvalType::DGNumber:
                     node->doubleValue = selected->doubleValue;
                     break;
                  case DGEvalType::DGBoolean:
                     node->boolValue = selected->boolValue;
                     break;
                  case DGEvalType::DGString:
                     node->stringValue = selected->stringValue;
                     break;
               }
               delete node->left;
               delete node->right->left;
               delete node->right->right;
               delete node->right;
               node->left = node->right = nullptr;
               transformed = true;
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

   for (DGEvalStatementNode *statement = statements->head; statement != nullptr; statement = statement->next) {
      if (statement->exp != nullptr) {
         // if ((optimization & OPTIMIZE_DC_STATEMENT) != 0) {
         //    if (statement->exp->functionCallCount == 0 && statement->exp->assignmentCount == 0) {
         //       continue;
         //    }
         // }
         scanForIC(nullptr, statement->exp);
      }
   }
   
   ic->emitIC(LRT, 8);
}

void DGEval::scanForIC(DGEvalExpNode *parentNode, DGEvalExpNode *node) {
   if (node == nullptr) {
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
         ic->emitIC(INSID, 0, node->type)->strConstant = node->stringValue;
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
    for (int i=0; i<code.size(); i++) {
        code.at(i).inst.writeAsJSON(outStream);
    }
}

void DGEvalICInst::writeAsJSON(ostream *outStream) {
   (*outStream) << "{";

   (*outStream) << "\"mnemonic\": \"" << DGEval::opStr[opCode]
                << "\", \"opCode\": " << opCode
                << ", \"p1\": " << p1
                << ", \"type\": \"" << (int)type.type
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
