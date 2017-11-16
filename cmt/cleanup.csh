if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /data/t2k/software/CMT/v1r20p20081118
endif
source ${CMTROOT}/mgr/setup.csh
set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=antiNumuCC1PiMinusAnalysis -version=v0r0 -path=/data/t2k/stewartt/T2K $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

