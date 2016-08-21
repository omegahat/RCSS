
getCSSRules =
function(nodes, sheet, eng = selectionEngine())  
{
  if(length(nodes) > 1)
    return(lapply(nodes,
            function(x) {
              getCSSRules(x, sheet, eng)
            }))

  .Call("R_getMatchedRulesets", eng, sheet, nodes)
}


selectionEngine =
function()
{
    .Call("R_createSelectionEngine")
}
