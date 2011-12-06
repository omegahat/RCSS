
getCSSRules =
function(nodes, sheet, eng = NULL)  
{
  if(length(nodes) > 1)
    return(lapply(nodes,
            function(x) {
              getCSSRules(x, sheet, eng)
            }))

  .Call("R_getMatchedRulesets", eng, sheet, nodes)
}
