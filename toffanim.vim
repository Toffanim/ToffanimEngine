"%% SiSU Vim color file
:set background=dark
:highlight clear
if version > 580
 hi clear
 if exists("syntax_on")
 syntax reset
 endif
endif
let colors_name = "toffanim"
:hi     Normal     guifg=#A08563     guibg=#161616
:hi     NonText    guifg=RoyalBlue   guibg=#161616
:hi     PreProc    guifg=#DAB98F
:hi     Include    guifg=#6B8E23
:hi     Comment    guifg=#7D7D7D     
:hi     Todo       guifg=#f2e530     guibg=#161616

:hi     Structure  guifg=#CD950C     gui=NONE
:hi     Statement  guifg=#CD950C     gui=NONE
:hi     Type       guifg=#CD950C     gui=NONE

:hi     Identifier guifg=salmon
:hi     Function   guifg=#DAB98F

:hi     String     guifg=#6B8E23 
:hi     Float      guifg=#6B8E23
:hi     Number     guifg=#6B8E23
:hi     Boolean    guifg=#6B8E23
:hi     Character  guifg=#6B8E23
:hi     Constant   guifg=#6B8E23
:hi     Tag        guifg=#DAB98F

:hi     Search     guibg=peru guifg=wheat cterm=none ctermfg=grey ctermbg=blue
:hi     IncSearch  guifg=green guibg=black cterm=none ctermfg=yellow ctermbg=green

:hi     LineNr     guifg=grey50
:set    guicursor=n-v-c-i:block-Cursor
:hi     iCursor    guibg=red
:hi     Cursor     guibg=green 
:set    guicursor+=i:iCursor 
:set    guicursor+=a:blinkon0
:hi     CursorLine guibg=#202020
:hi     StatusLine guibg=#c2bfa5 guifg=black gui=none cterm=bold,reverse
:hi     StatusLineNC guibg=#161616 guifg=grey40 gui=none cterm=reverse
:hi     MatchParen  guibg=salmon      guifg=#161616
:hi     SpecialKey  guifg=yellowgreen
:hi     VertSplit   guibg=#c2bfa5 guifg=grey40 gui=none cterm=reverse
:hi     Visual      gui=none guifg=khaki guibg=olivedrab cterm=reverse

:hi     Pmenu       guifg=RoyalBlue guibg=#202020
:hi     PmenuSel    guifg=#202020  guibg=RoyalBlue

" =================================================================================
"  @Cleanup : change this color if something if off
" =================================================================================
:hi Folded guibg=black guifg=grey40 ctermfg=grey ctermbg=darkgrey
:hi FoldColumn guibg=black guifg=grey20 ctermfg=4 ctermbg=7
:hi ModeMsg guifg=goldenrod cterm=none ctermfg=brown
:hi MoreMsg guifg=SeaGreen ctermfg=darkgreen

:hi Question guifg=springgreen ctermfg=green

:hi Title guifg=gold gui=bold cterm=bold ctermfg=yellow

:hi WarningMsg guifg=salmon ctermfg=1
:hi Special guifg=darkkhaki ctermfg=brown
:hi Operator guifg=Red ctermfg=Red
:hi Ignore guifg=grey40 cterm=bold ctermfg=7
:hi Directory ctermfg=darkcyan
:hi ErrorMsg cterm=bold guifg=White guibg=Red cterm=bold ctermfg=7 ctermbg=1
:hi VisualNOS cterm=bold,underline
:hi WildMenu ctermfg=0 ctermbg=3
:hi DiffAdd ctermbg=4
:hi DiffChange ctermbg=5
:hi DiffDelete cterm=bold ctermfg=4 ctermbg=6
:hi DiffText cterm=bold ctermbg=1
:hi Underlined cterm=underline ctermfg=5
:hi Error guifg=White guibg=Red cterm=bold ctermfg=7 ctermbg=1
:hi SpellErrors guifg=White guibg=Red cterm=bold ctermfg=7 ctermbg=1

