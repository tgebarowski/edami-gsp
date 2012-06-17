(TeX-add-style-hook "gsp-paper"
 (lambda ()
    (LaTeX-add-bibliographies
     "mybib")
    (LaTeX-add-environments
     "my")
    (LaTeX-add-labels
     "figure:gsp-class-diagram"
     "figure:input-format"
     "tab:Res")
    (TeX-add-symbols
     "IEEEkeywordsname")
    (TeX-run-style-hooks
     "listings"
     "ieeefig"
     "latex2e"
     "IEEEtran10"
     "IEEEtran"
     "journal"
     "a4paper")))

