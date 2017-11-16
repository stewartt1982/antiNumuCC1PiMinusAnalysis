#include "antiNumuCC1PiMinusAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  antiNumuCC1PiMinusAnalysis* ana = new antiNumuCC1PiMinusAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
