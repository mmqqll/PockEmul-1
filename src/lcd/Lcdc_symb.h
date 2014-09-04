#include <QtCore>

static const char
*S_REV_BATT=
"#################\r"
"#  ### ##   #   #\r"
"# # # # ## ### ##\r"
"#  ##   ## ### ##\r"
"# # # # ## ### ##\r"
"#  ## # ## ### ##\r"
"#################"

,*S_BUSY=

"##  #  #  ## # #\r"
"# # #  # #   # #\r"
"##  #  #  #  # #\r"
"# # #  #   #  # \r"
"##   ##  ##   #"

,*S_HYP=

"# # # # ## \r"
"# # # # # #\r"
"###  #  ## \r"
"# #  #  #  \r"
"# #  #  #  "

,*S_O_BRA=

" #\r"
"# \r"
"# \r"
"# \r"
" #"

,*S_C_BRA=

"# \r"
" #\r"
" #\r"
" #\r"
"#"

,*S_REV_M=

"#######\r"
"#  #  #\r"
"# # # #\r"
"# ### #\r"
"#######"

,*S_E=

"####\r"
"#  \r"
"###\r"
"#  \r"
"####"

,*S_PRINT=

"### \r"
"#  #\r"
"###\r"
"#  \r"
"#   "

,*S_PRINTL=

"###  ###  # #   # #####\r"
"#  # #  # # ##  #   #  \r"
"###  ###  # # # #   #  \r"
"#    # #  # #  ##   #  \r"
"#    #  # # #   #   #  "

,*S_DBL=

"##  ##  #  \r"
"# # # # #  \r"
"# # ##  #  \r"
"# # # # #  \r"
"##  ##  ###"

,*S_LOW=

"##############\r"
"# ### ## # # #\r"
"# ## # # # # #\r"
"## ## ### # ##\r"
"##############"

,*S_INF=

"  #\r"
" # \r"
"#  \r"
" # \r"
"  #"

,*S_DA=

"#######\r"
" #   # \r"
"  # #  \r"
"   #   \r"
"       "

,*S_SUP=

"#  \r"
" # \r"
"  #\r"
" # \r"
"#  "

,*S_UCL=

"# #  ## #  \r"
"# # #   #  \r"
"# # #   #  \r"
"# # #   #  \r"
"###  ##  ##"

,*S_CTL=

" ## ### #  \r"
"#    #  #  \r"
"#    #  #  \r"
"#    #  #  \r"
" ##  #   ##"

,*S_FN=

"### #  #\r"
"#   ## #\r"
"##  # ##\r"
"#   #  #\r"
"#   #  #"

,*S_IO=

"###   # ###  \r"
" #    # # #  \r"
" #   #  # #  \r"
" #  #   # #  \r"
"### #   ###"

,*S_SHIFT=

" ## # # # ### ###\r"
"#   # # # #    # \r"
" #  ### # ###  # \r"
"  # # # # #    # \r"
"##  # # # #    #"

,*S_ERROR=

" ## ##  ##   #  ## \r"
"#   # # # # # # # #\r"
"### ##  ##  # # ## \r"
"#   # # # # # # # #\r"
" ## # # # #  #  # #"

,*S_CTRL=

" ## ### ##  #  \r"
"#    #  # # #  \r"
"#    #  ##  #  \r"
"#    #  # # #  \r"
" ##  #  # # ###"

,*S_SML=

" ## #   # #  \r"
"#   ## ## #  \r"
" #  # # # #  \r"
"  # #   # #  \r"
"##  #   # ###"

,*S_STAT=

" ## ###  #  ###\r"
"#    #  # #  # \r"
" #   #  ###  # \r"
"  #  #  # #  # \r"
"##   #  # #  #"

,*S_SMALL=

" ## #   #  #  #   #  \r"
"#   ## ## # # #   #  \r"
" #  # # # ### #   #  \r"
"  # #   # # # #   #  \r"
"##  #   # # # ### ###"

,*S_CASL=

" ##  #   ## #  \r"
"#   # # #   #  \r"
"#   ###  #  #  \r"
"#   # #   # #  \r"
" ## # # ##  ###"

,*S_CONST=

" ##  #  #  #  ## ###\r"
"#   # # ## # #    #\r"
"#   # # # ##  #   #\r"
"#   # # #  #   #  #\r"
" ##  #  #  # ##   #"

,*S_CAPS=

" ##  #  ###  ##\r"
"#   # # #  ##  \r"
"#   ### ###  # \r"
"#   # # #     #\r"
" ## # # #   ## "

,*S_SECF=

" ##            ###\r"
"#  # # #     # #  \r"
"  #  ## #  ### ###\r"
" #   #  # #  # #  \r"
"###  #  #  ### #  "

,*S_SEC=

" ##           \r"
"#  # # #     #\r"
"  #  ## #  ###\r"
" #   #  # #  #\r"
"###  #  #  ###"

,*S_JAP=

" #      # \r"
"##### ####\r"
" # #    # \r"
" # #    # \r"
"# #    #  "

,*S_JAP2=

"   #   \r"
" # # # \r"
"#  #  #\r"
"   #   \r"
"  #    "

,*S_TEXT=

"### ### # # ###\r"
" #  #   # #  # \r"
" #  ###  #   # \r"
" #  #   # #  # \r"
" #  ### # #  #"

,*S_DE=

"##  ###\r"
"# # #  \r"
"# # ###\r"
"# # #  \r"
"##  ###"

,*S_G=
" ###\r"
"#   \r"
"# ##\r"
"#  #\r"
" ##"

,*S_DEG=

"##  ###  ###\r"
"# # #   #   \r"
"# # ### # ##\r"
"# # #   #  #\r"
"##  ###  ##"


,*S_RAD=

"##   #  ## \r"
"# # # # # #\r"
"##  ### # #\r"
"# # # # # #\r"
"# # # # ##"

,*S_RESERVE=

"##  ###  ## ### ##  # # ###\r"
"# # #   #   #   # # # # #  \r"
"##  ###  #  ### ##  # # ###\r"
"# # #     # #   # # # # #  \r"
"# # ### ##  ### # #  #  ###"

,*S_RUN=

"##  #  # #  #\r"
"# # #  # ## #\r"
"##  #  # # ##\r"
"# # #  # #  #\r"
"# #  ##  #  #"

,*S_PRO=

"##  ##   # \r"
"# # # # # #\r"
"##  ##  # #\r"
"#   # # # #\r"
"#   # #  #"

,*S_DEF=

"##  ### ###\r"
"# # #   #  \r"
"# # ### ###\r"
"# # #   #  \r"
"##  ### #"

,*S_ROMEAN_I =

"###\r"
" # \r"
" # \r"
" # \r"
"###"

,*S_ROMEAN_II=

"######\r"
" #  # \r"
" #  # \r"
" #  # \r"
"######"

,*S_ROMEAN_III=

"#########\r"
" #  #  # \r"
" #  #  # \r"
" #  #  # \r"
"#########"

,*S_BATTERY=

" ## \r"
"####\r"
"####\r"
" ##"

,*S_BAR13=

"###"

,*S_BAR23=

"###\r"
"###"

,*S_BAR33=

"###\r"
"###\r"
"###"

,*S_BAR43=

"###\r"
"###\r"
"###\r"
"###"

,*S_BAR53=

"###\r"
"###\r"
"###\r"
"###\r"
"###"

,*S_BAR14=

"####"

,*S_BAR24=

"####\r"
"####"

,*S_BAR34=

"####\r"
"####\r"
"####"

,*S_BAR44=

"####\r"
"####\r"
"####\r"
"####"

,*S_BAR54=

"####\r"
"####\r"
"####\r"
"####\r"
"####"

,*S_BAR15=

"#####"

,*S_BAR25=

"#####\r"
"#####"

,*S_BAR35=

"#####\r"
"#####\r"
"#####"

,*S_BAR45=

"#####\r"
"#####\r"
"#####\r"
"#####"

,*S_BAR55=

"#####\r"
"#####\r"
"#####\r"
"#####\r"
"#####"

;


