# echo "Setting antiNumuCC1PiMinusAnalysis v0r0 in /data/t2k/stewartt/T2K"

if test "${CMTROOT}" = ""; then
  CMTROOT=/data/t2k/software/CMT/v1r20p20081118; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=antiNumuCC1PiMinusAnalysis -version=v0r0 -path=/data/t2k/stewartt/T2K  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

