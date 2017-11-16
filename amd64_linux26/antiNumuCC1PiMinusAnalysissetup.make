----------> uses
# use antiNumuCCAnalysis * highland2
#   use numuCCAnalysis * highland2
#     use baseTrackerAnalysis * highland2
#       use baseAnalysis * highland2
#         use highlandIO * highland2
#           use psycheIO * psyche
#             use psycheCore * psyche
#               use psychePolicy * psyche
#               use ROOT * 
#                 use EXTERN * 
#                 use MYSQL * 
#                   use EXTERN * 
#                 use GSL * 
#                   use EXTERN * 
#             use psycheND280Utils * psyche
#               use psycheUtils * psyche
#                 use psycheEventModel * psyche
#                   use psycheCore * psyche
#           use highlandCorrections * highland2
#             use highlandUtils * highland2
#               use highlandCore * highland2
#                 use psycheCore * psyche
#               use highlandEventModel * highland2
#                 use psycheEventModel * psyche
#                 use oaAnalysisReader * highland2
#                   use ROOT * 
#               use psycheND280Utils * psyche
#             use psycheSystematics * psyche
#               use psycheSelections * psyche
#                 use psycheCore * psyche
#                 use psycheEventModel * psyche
#                 use psycheIO * psyche
#         use highlandCorrections * highland2
#         use psycheSelections * psyche
#         use psycheSystematics * psyche
#         use highlandTools * highland2
#           use highlandCore * highland2
#
# Selection :
use CMT v1r20p20081118 (/data/t2k/software)
use EXTERN v3r7  (/data/t2k/software/nd280_6B/work)
use GSL v1r15p0n00  (/data/t2k/software/nd280_6B/work)
use MYSQL v5r051an01  (/data/t2k/software/nd280_6B/work)
use ROOT v5r34p09n04  (/data/t2k/software/nd280_6B/work)
use oaAnalysisReader v2r3 highland2 (/data/t2k/stewartt/T2K/highland_v2r15/)
use psychePolicy v3r1 psyche (/data/t2k/stewartt/T2K/highland_v2r15/)
use psycheCore v3r15 psyche (/data/t2k/stewartt/T2K/highland_v2r15/)
use highlandCore v2r15 highland2 (/data/t2k/stewartt/T2K/highland_v2r15/)
use highlandTools v2r13 highland2 (/data/t2k/stewartt/T2K/highland_v2r15/)
use psycheEventModel v3r15 psyche (/data/t2k/stewartt/T2K/highland_v2r15/)
use highlandEventModel v2r13 highland2 (/data/t2k/stewartt/T2K/highland_v2r15/)
use psycheUtils v3r17 psyche (/data/t2k/stewartt/T2K/highland_v2r15/)
use psycheND280Utils v3r17 psyche (/data/t2k/stewartt/T2K/highland_v2r15/)
use highlandUtils v2r15 highland2 (/data/t2k/stewartt/T2K/highland_v2r15/)
use psycheIO v3r13 psyche (/data/t2k/stewartt/T2K/highland_v2r15/)
use psycheSelections v3r17 psyche (/data/t2k/stewartt/T2K/highland_v2r15/)
use psycheSystematics v3r17 psyche (/data/t2k/stewartt/T2K/highland_v2r15/)
use highlandCorrections v2r7 highland2 (/data/t2k/stewartt/T2K/highland_v2r15/)
use highlandIO v2r15 highland2 (/data/t2k/stewartt/T2K/highland_v2r15/)
use baseAnalysis v2r13 highland2 (/data/t2k/stewartt/T2K/highland_v2r15/)
use baseTrackerAnalysis v2r11 highland2 (/data/t2k/stewartt/T2K/highland_v2r15/)
use numuCCAnalysis v2r13 highland2 (/data/t2k/stewartt/T2K/highland_v2r15/)
use antiNumuCCAnalysis v2r3 highland2 (/data/t2k/stewartt/T2K/highland_v2r15/)
----------> tags
CMTv1 (from CMTVERSION)
CMTr20 (from CMTVERSION)
CMTp20081118 (from CMTVERSION)
Linux (from uname) package CMT implies [Unix]
amd64_linux26 (from CMTCONFIG)
T2K_config (from PROJECT) excludes [T2K_no_config]
T2K_root (from PROJECT) excludes [T2K_no_root]
T2K_cleanup (from PROJECT) excludes [T2K_no_cleanup]
T2K_prototypes (from PROJECT) excludes [T2K_no_prototypes]
T2K_without_installarea (from PROJECT) excludes [T2K_with_installarea]
T2K_with_version_directory (from PROJECT) excludes [T2K_without_version_directory]
nd280_6B_config (from PROJECT) excludes [nd280_6B_no_config]
nd280_6B_root (from PROJECT) excludes [nd280_6B_no_root]
nd280_6B_cleanup (from PROJECT) excludes [nd280_6B_no_cleanup]
nd280_6B_prototypes (from PROJECT) excludes [nd280_6B_no_prototypes]
nd280_6B_without_installarea (from PROJECT) excludes [nd280_6B_with_installarea]
nd280_6B_with_version_directory (from PROJECT) excludes [nd280_6B_without_version_directory]
x86_64 (from package CMT)
slc69 (from package CMT)
gcc447 (from package CMT)
Unix (from package CMT) excludes [WIN32 Win32]
----------> CMTPATH
# Add path /data/t2k/stewartt/T2K/highland_v2r15 from initialization
# Add path /data/t2k/software/nd280_6B/work from initialization
