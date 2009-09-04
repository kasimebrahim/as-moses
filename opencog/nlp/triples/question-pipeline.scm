;
; question-pipeline.scm
; 
; An experimental set of question-answering rules
;
; Copyright (c) 2009 Linas Vepstas <linasvepstas@gmail.com>
;
; -----------------------------------------------------------------
; The following answers a simple WH-question (who, what, when etc.)
; The question is presumed to be a simple triple itself.
;
; To limit the scope of the search (for performance), the bottom
; of the prep-triple is assumed to be anchored.
;
; Basically, we are trying to handle triples of the form
; "capital_of(France, what)" and verifying that "what" is a query,
; and then yanking out the answer.
; 
; # IF %ListLink("# TRIPLE BOTTOM ANCHOR", $qvar) 
;       ^ $prep($word-inst, $qvar)      ; the question
;       ^ &query_var($qvar)             ; validate WH-question
;       ^ %LemmaLink($word-inst, $word) ; common word-instance
;       ^ %LemmaLink($join-inst, $word) ; common seme
;       ^ $prep($join-inst, $ans)       ; answer
;       ^ ! &query_var($ans)            ; answer should NOT be a query
;    THEN
;       ^3_&declare_answer($ans)
 
(define (wh-question wh-clause ans-clause)
	(r-varscope
		(r-and
			(r-anchor "# TRIPLE BOTTOM ANCHOR" "$qvar")

			wh-clause  ; the prep-phrase we are matching!

			;; XXX someday, this needs to be an or-list of WH- words.
			(r-rlx-flag "what" "$qvar")
			(r-decl-lemma  "$word-inst" "$word")
			(r-decl-lemma  "$join-inst" "$word")

			ans-clause

			(r-not (r-rlx-flag "what" "$ans"))
		)
		(r-anchor "# QUERY SOLUTION" "$ans")
	)
)

(define question-rule-0
	(wh-question 
		(r-rlx "$prep" "$word-inst" "$qvar")
		(r-rlx "$prep" "$join-inst" "$ans")
	)
)

(define question-rule-1
	(wh-question 
		(r-rlx "$prep" "$qvar" "$word-inst")
		(r-rlx "$prep" "$ans"  "$join-inst")
	)
)

; -----------------------------------------------------------------
; # IF %ListLink("# TRIPLE BOTTOM ANCHOR", $qvar) 
;       ^ $prep($qvar, $var)    ; the question
;       ^ &query_var($qvar)     ; validate WH-question
;       ^ $prep($ans, $var)     ; answer
;    THEN
;       ^3_&declare_answer($ans)

(define question-rule-2
	(r-varscope
		(r-and
			(r-anchor "# TRIPLE BOTTOM ANCHOR" "$qvar")
			(r-rlx "$prep" "$qvar" "$var")

			;; XXX someday, this needs to be an or-list of WH- words.
			(r-rlx-flag "what" "$qvar")
			(r-rlx "$prep" "$ans" "$var")
		)
		(r-anchor "# QUERY SOLUTION" "$ans")
	)
)

; -----------------------------------------------------------------
;
(define *question-rule-list* (list
	question-rule-0
	question-rule-1
;	question-rule-2
))

; ------------------------ END OF FILE ----------------------------
; -----------------------------------------------------------------

