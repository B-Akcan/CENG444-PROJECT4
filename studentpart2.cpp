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
   return;
}

void DGEval::scanConstantFolding(DGEvalExpNode *parentNode, DGEvalExpNode *node) {
   return;
}

void DGEval::scanForIC() {
   return;
}

void DGEval::scanForIC(DGEvalExpNode *parentNode, DGEvalExpNode *node) {
   return;
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
