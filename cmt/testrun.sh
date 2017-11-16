
source /home/stewartt/t2ksoft_highland_v2r15.sh
cd /data/t2k/stewartt/T2K/antiNumuCC1PiMinusAnalysis/v0r0/cmt
source setup.sh
cd /data/t2k/stewartt/T2K/antiNumuCC1PiMinusAnalysis/v0r0/cmt/antiNumuCC1PiMinusAnalysis_output/job

date

/data/t2k/stewartt/T2K/antiNumuCC1PiMinusAnalysis/v0r0/amd64_linux26/RunAntiNumuCC1PiMinusAnalysis.exe -o ../AnaOut_sub_testpionPID.root /data/t2k/stewartt/T2K/antiNumuCC1PiMinusAnalysis/v0r0/cmt/testfiles.txt

date

