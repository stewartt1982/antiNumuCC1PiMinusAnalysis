#-- start of make_header -----------------

#====================================
#  Library antiNumuCC1PiMinusAnalysis
#
#   Generated Thu Jul  6 15:28:40 2017  by stewartt
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_antiNumuCC1PiMinusAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_antiNumuCC1PiMinusAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_antiNumuCC1PiMinusAnalysis

antiNumuCC1PiMinusAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_antiNumuCC1PiMinusAnalysis = /tmp/CMT_$(antiNumuCC1PiMinusAnalysis_tag)_antiNumuCC1PiMinusAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_antiNumuCC1PiMinusAnalysis = $(antiNumuCC1PiMinusAnalysis_tag)_antiNumuCC1PiMinusAnalysis.make
cmt_local_tagfile_antiNumuCC1PiMinusAnalysis = $(bin)$(antiNumuCC1PiMinusAnalysis_tag)_antiNumuCC1PiMinusAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

antiNumuCC1PiMinusAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_antiNumuCC1PiMinusAnalysis = /tmp/CMT_$(antiNumuCC1PiMinusAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_antiNumuCC1PiMinusAnalysis = $(antiNumuCC1PiMinusAnalysis_tag).make
cmt_local_tagfile_antiNumuCC1PiMinusAnalysis = $(bin)$(antiNumuCC1PiMinusAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_antiNumuCC1PiMinusAnalysis)

ifdef cmt_antiNumuCC1PiMinusAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_antiNumuCC1PiMinusAnalysis = /tmp/CMT_antiNumuCC1PiMinusAnalysis_antiNumuCC1PiMinusAnalysissetup.make
cmt_local_antiNumuCC1PiMinusAnalysis_makefile = /tmp/CMT_antiNumuCC1PiMinusAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_antiNumuCC1PiMinusAnalysis = $(bin)antiNumuCC1PiMinusAnalysis_antiNumuCC1PiMinusAnalysissetup.make
cmt_local_antiNumuCC1PiMinusAnalysis_makefile = $(bin)antiNumuCC1PiMinusAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_antiNumuCC1PiMinusAnalysis = /tmp/CMT_antiNumuCC1PiMinusAnalysissetup.make
cmt_local_antiNumuCC1PiMinusAnalysis_makefile = /tmp/CMT_antiNumuCC1PiMinusAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_antiNumuCC1PiMinusAnalysis = $(bin)antiNumuCC1PiMinusAnalysissetup.make
cmt_local_antiNumuCC1PiMinusAnalysis_makefile = $(bin)antiNumuCC1PiMinusAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_antiNumuCC1PiMinusAnalysissetup.make
else
cmt_final_setup = $(bin)antiNumuCC1PiMinusAnalysissetup.make
endif

antiNumuCC1PiMinusAnalysis ::


ifdef READONLY
antiNumuCC1PiMinusAnalysis ::
	@echo tags=$(tags)
	@echo cmt_local_tagfile=$(cmt_local_tagfile)
endif


dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'antiNumuCC1PiMinusAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = antiNumuCC1PiMinusAnalysis/
antiNumuCC1PiMinusAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

antiNumuCC1PiMinusAnalysislibname   = $(bin)$(library_prefix)antiNumuCC1PiMinusAnalysis$(library_suffix)
antiNumuCC1PiMinusAnalysislib       = $(antiNumuCC1PiMinusAnalysislibname).a
antiNumuCC1PiMinusAnalysisstamp     = $(bin)antiNumuCC1PiMinusAnalysis.stamp
antiNumuCC1PiMinusAnalysisshstamp   = $(bin)antiNumuCC1PiMinusAnalysis.shstamp

antiNumuCC1PiMinusAnalysis :: dirs  antiNumuCC1PiMinusAnalysisLIB
	$(echo) "antiNumuCC1PiMinusAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

antiNumuCC1PiMinusAnalysisLIB :: $(antiNumuCC1PiMinusAnalysislib) $(antiNumuCC1PiMinusAnalysisshstamp)
	$(echo) "antiNumuCC1PiMinusAnalysis : library ok"

$(antiNumuCC1PiMinusAnalysislib) :: $(bin)antiNumuCC1PiMinusSelection.o $(bin)antiNumuCC1PiMinusAnalysis.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(antiNumuCC1PiMinusAnalysislib) $?
	$(lib_silent) $(ranlib) $(antiNumuCC1PiMinusAnalysislib)
	$(lib_silent) cat /dev/null >$(antiNumuCC1PiMinusAnalysisstamp)

#------------------------------------------------------------------
#  Future improvement? to empty the object files after
#  storing in the library
#
##	  for f in $?; do \
##	    rm $${f}; touch $${f}; \
##	  done
#------------------------------------------------------------------

#
# We add one level of dependency upon the true shared library 
# (rather than simply upon the stamp file)
# this is for cases where the shared library has not been built
# while the stamp was created (error??) 
#

$(antiNumuCC1PiMinusAnalysislibname).$(shlibsuffix) :: $(antiNumuCC1PiMinusAnalysislib) requirements $(use_requirements) $(antiNumuCC1PiMinusAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" antiNumuCC1PiMinusAnalysis $(antiNumuCC1PiMinusAnalysis_shlibflags)

$(antiNumuCC1PiMinusAnalysisshstamp) :: $(antiNumuCC1PiMinusAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(antiNumuCC1PiMinusAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(antiNumuCC1PiMinusAnalysisshstamp) ; fi

antiNumuCC1PiMinusAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)antiNumuCC1PiMinusSelection.o $(bin)antiNumuCC1PiMinusAnalysis.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf antiNumuCC1PiMinusAnalysis_deps antiNumuCC1PiMinusAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
antiNumuCC1PiMinusAnalysisinstallname = $(library_prefix)antiNumuCC1PiMinusAnalysis$(library_suffix).$(shlibsuffix)

antiNumuCC1PiMinusAnalysis :: antiNumuCC1PiMinusAnalysisinstall

install :: antiNumuCC1PiMinusAnalysisinstall

antiNumuCC1PiMinusAnalysisinstall :: $(install_dir)/$(antiNumuCC1PiMinusAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(antiNumuCC1PiMinusAnalysisinstallname) :: $(bin)$(antiNumuCC1PiMinusAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(antiNumuCC1PiMinusAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##antiNumuCC1PiMinusAnalysisclean :: antiNumuCC1PiMinusAnalysisuninstall

uninstall :: antiNumuCC1PiMinusAnalysisuninstall

antiNumuCC1PiMinusAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(antiNumuCC1PiMinusAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),antiNumuCC1PiMinusAnalysisclean)

#$(bin)antiNumuCC1PiMinusAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)antiNumuCC1PiMinusAnalysis_dependencies.make : ../src/antiNumuCC1PiMinusSelection.cxx ../src/antiNumuCC1PiMinusAnalysis.cxx $(use_requirements) $(cmt_final_setup_antiNumuCC1PiMinusAnalysis)
	$(echo) "(antiNumuCC1PiMinusAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) antiNumuCC1PiMinusAnalysis -all_sources -out=$@ ../src/antiNumuCC1PiMinusSelection.cxx ../src/antiNumuCC1PiMinusAnalysis.cxx
endif

#$(antiNumuCC1PiMinusAnalysis_dependencies)

-include $(bin)antiNumuCC1PiMinusAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)antiNumuCC1PiMinusAnalysis_dependencies.make : $(antiNumuCC1PiMinusSelection_cxx_dependencies)

$(bin)$(binobj)antiNumuCC1PiMinusSelection.o : $(antiNumuCC1PiMinusSelection_cxx_dependencies)
	$(cpp_echo) $(src)antiNumuCC1PiMinusSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(antiNumuCC1PiMinusAnalysis_pp_cppflags) $(lib_antiNumuCC1PiMinusAnalysis_pp_cppflags) $(antiNumuCC1PiMinusSelection_pp_cppflags) $(use_cppflags) $(antiNumuCC1PiMinusAnalysis_cppflags) $(lib_antiNumuCC1PiMinusAnalysis_cppflags) $(antiNumuCC1PiMinusSelection_cppflags) $(antiNumuCC1PiMinusSelection_cxx_cppflags)  $(src)antiNumuCC1PiMinusSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)antiNumuCC1PiMinusAnalysis_dependencies.make : $(antiNumuCC1PiMinusAnalysis_cxx_dependencies)

$(bin)$(binobj)antiNumuCC1PiMinusAnalysis.o : $(antiNumuCC1PiMinusAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)antiNumuCC1PiMinusAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(antiNumuCC1PiMinusAnalysis_pp_cppflags) $(lib_antiNumuCC1PiMinusAnalysis_pp_cppflags) $(antiNumuCC1PiMinusAnalysis_pp_cppflags) $(use_cppflags) $(antiNumuCC1PiMinusAnalysis_cppflags) $(lib_antiNumuCC1PiMinusAnalysis_cppflags) $(antiNumuCC1PiMinusAnalysis_cppflags) $(antiNumuCC1PiMinusAnalysis_cxx_cppflags)  $(src)antiNumuCC1PiMinusAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: antiNumuCC1PiMinusAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(antiNumuCC1PiMinusAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(antiNumuCC1PiMinusAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_antiNumuCC1PiMinusAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNumuCC1PiMinusAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNumuCC1PiMinusAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNumuCC1PiMinusAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

antiNumuCC1PiMinusAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)antiNumuCC1PiMinusAnalysis$(library_suffix).a $(binobj)$(library_prefix)antiNumuCC1PiMinusAnalysis$(library_suffix).s? $(binobj)antiNumuCC1PiMinusAnalysis.stamp $(binobj)antiNumuCC1PiMinusAnalysis.shstamp
#-- end of cleanup_library ---------------

