#-- start of make_header -----------------

#====================================
#  Application RunAntiNumuCC1PiMinusAnalysis
#
#   Generated Thu Jul  6 15:28:48 2017  by stewartt
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunAntiNumuCC1PiMinusAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunAntiNumuCC1PiMinusAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunAntiNumuCC1PiMinusAnalysis

antiNumuCC1PiMinusAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunAntiNumuCC1PiMinusAnalysis = /tmp/CMT_$(antiNumuCC1PiMinusAnalysis_tag)_RunAntiNumuCC1PiMinusAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunAntiNumuCC1PiMinusAnalysis = $(antiNumuCC1PiMinusAnalysis_tag)_RunAntiNumuCC1PiMinusAnalysis.make
cmt_local_tagfile_RunAntiNumuCC1PiMinusAnalysis = $(bin)$(antiNumuCC1PiMinusAnalysis_tag)_RunAntiNumuCC1PiMinusAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

antiNumuCC1PiMinusAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunAntiNumuCC1PiMinusAnalysis = /tmp/CMT_$(antiNumuCC1PiMinusAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunAntiNumuCC1PiMinusAnalysis = $(antiNumuCC1PiMinusAnalysis_tag).make
cmt_local_tagfile_RunAntiNumuCC1PiMinusAnalysis = $(bin)$(antiNumuCC1PiMinusAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunAntiNumuCC1PiMinusAnalysis)

ifdef cmt_RunAntiNumuCC1PiMinusAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunAntiNumuCC1PiMinusAnalysis = /tmp/CMT_antiNumuCC1PiMinusAnalysis_RunAntiNumuCC1PiMinusAnalysissetup.make
cmt_local_RunAntiNumuCC1PiMinusAnalysis_makefile = /tmp/CMT_RunAntiNumuCC1PiMinusAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunAntiNumuCC1PiMinusAnalysis = $(bin)antiNumuCC1PiMinusAnalysis_RunAntiNumuCC1PiMinusAnalysissetup.make
cmt_local_RunAntiNumuCC1PiMinusAnalysis_makefile = $(bin)RunAntiNumuCC1PiMinusAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunAntiNumuCC1PiMinusAnalysis = /tmp/CMT_antiNumuCC1PiMinusAnalysissetup.make
cmt_local_RunAntiNumuCC1PiMinusAnalysis_makefile = /tmp/CMT_RunAntiNumuCC1PiMinusAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunAntiNumuCC1PiMinusAnalysis = $(bin)antiNumuCC1PiMinusAnalysissetup.make
cmt_local_RunAntiNumuCC1PiMinusAnalysis_makefile = $(bin)RunAntiNumuCC1PiMinusAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_antiNumuCC1PiMinusAnalysissetup.make
else
cmt_final_setup = $(bin)antiNumuCC1PiMinusAnalysissetup.make
endif

RunAntiNumuCC1PiMinusAnalysis ::


ifdef READONLY
RunAntiNumuCC1PiMinusAnalysis ::
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
	$(echo) 'RunAntiNumuCC1PiMinusAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunAntiNumuCC1PiMinusAnalysis/
RunAntiNumuCC1PiMinusAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunAntiNumuCC1PiMinusAnalysis :: dirs  $(bin)RunAntiNumuCC1PiMinusAnalysis${application_suffix}
	$(echo) "RunAntiNumuCC1PiMinusAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunAntiNumuCC1PiMinusAnalysis${application_suffix} :: $(bin)RunAntiNumuCC1PiMinusAnalysis.o $(use_stamps) $(RunAntiNumuCC1PiMinusAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunAntiNumuCC1PiMinusAnalysis.o $(cmt_installarea_linkopts) $(RunAntiNumuCC1PiMinusAnalysis_use_linkopts) $(RunAntiNumuCC1PiMinusAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunAntiNumuCC1PiMinusAnalysisinstallname = RunAntiNumuCC1PiMinusAnalysis${application_suffix}

RunAntiNumuCC1PiMinusAnalysis :: RunAntiNumuCC1PiMinusAnalysisinstall

install :: RunAntiNumuCC1PiMinusAnalysisinstall

RunAntiNumuCC1PiMinusAnalysisinstall :: $(install_dir)/$(RunAntiNumuCC1PiMinusAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunAntiNumuCC1PiMinusAnalysisinstallname) :: $(bin)$(RunAntiNumuCC1PiMinusAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunAntiNumuCC1PiMinusAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunAntiNumuCC1PiMinusAnalysisclean :: RunAntiNumuCC1PiMinusAnalysisuninstall

uninstall :: RunAntiNumuCC1PiMinusAnalysisuninstall

RunAntiNumuCC1PiMinusAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunAntiNumuCC1PiMinusAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunAntiNumuCC1PiMinusAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunAntiNumuCC1PiMinusAnalysisclean)

#$(bin)RunAntiNumuCC1PiMinusAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunAntiNumuCC1PiMinusAnalysis_dependencies.make : ../app/RunAntiNumuCC1PiMinusAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunAntiNumuCC1PiMinusAnalysis)
	$(echo) "(RunAntiNumuCC1PiMinusAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunAntiNumuCC1PiMinusAnalysis -all_sources -out=$@ ../app/RunAntiNumuCC1PiMinusAnalysis.cxx
endif

#$(RunAntiNumuCC1PiMinusAnalysis_dependencies)

-include $(bin)RunAntiNumuCC1PiMinusAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunAntiNumuCC1PiMinusAnalysis_dependencies.make : $(RunAntiNumuCC1PiMinusAnalysis_cxx_dependencies)

$(bin)$(binobj)RunAntiNumuCC1PiMinusAnalysis.o : $(RunAntiNumuCC1PiMinusAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunAntiNumuCC1PiMinusAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunAntiNumuCC1PiMinusAnalysis_pp_cppflags) $(app_RunAntiNumuCC1PiMinusAnalysis_pp_cppflags) $(RunAntiNumuCC1PiMinusAnalysis_pp_cppflags) $(use_cppflags) $(RunAntiNumuCC1PiMinusAnalysis_cppflags) $(app_RunAntiNumuCC1PiMinusAnalysis_cppflags) $(RunAntiNumuCC1PiMinusAnalysis_cppflags) $(RunAntiNumuCC1PiMinusAnalysis_cxx_cppflags) -I../app ../app/RunAntiNumuCC1PiMinusAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunAntiNumuCC1PiMinusAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunAntiNumuCC1PiMinusAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunAntiNumuCC1PiMinusAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunAntiNumuCC1PiMinusAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNumuCC1PiMinusAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNumuCC1PiMinusAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNumuCC1PiMinusAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunAntiNumuCC1PiMinusAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunAntiNumuCC1PiMinusAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunAntiNumuCC1PiMinusAnalysis${application_suffix}

#	@echo "------> (RunAntiNumuCC1PiMinusAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunAntiNumuCC1PiMinusAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunAntiNumuCC1PiMinusAnalysis_deps RunAntiNumuCC1PiMinusAnalysis_dependencies.make
#-- end of cleanup_objects ------
