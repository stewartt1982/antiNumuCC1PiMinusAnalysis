#!/bin/bash
ANTINUMUCC1PIMINUSANALYSISROOT="/data/t2k/stewartt/T2K/antiNumuCC1PiMinusAnalysis/v0r0" 
export ANTINUMUCC1PIMINUSANALYSISROOT
ANTINUMUCC1PIMINUSANALYSISCONFIG="amd64_linux26" 
export ANTINUMUCC1PIMINUSANALYSISCONFIG
export PATH=$ANTINUMUCC1PIMINUSANALYSISROOT/$ANTINUMUCC1PIMINUSANALYSISCONFIG:$PATH
export LD_LIBRARY_PATH=$ANTINUMUCC1PIMINUSANALYSISROOT/$ANTINUMUCC1PIMINUSANALYSISCONFIG:$LD_LIBRARY_PATH