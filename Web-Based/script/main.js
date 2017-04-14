"use strict";(function(){var c=new ScrollMagic.Controller,f=function(g){return new TweenMax.to(g,1.5,{opacity:0})};new ScrollMagic.Scene({duration:100,offset:0}).setPin("#headerWrapper").addTo(c),new ScrollMagic.Scene({duration:50,offset:10}).setTween(f("#instructions")).addTo(c),new ScrollMagic.Scene({duration:200,offset:100}).setTween(f("#headerWrapper")).setPin("#headerWrapper").addTo(c)})(),function(){function c(p,q){$(p).click(function(){$("html, body").animate({scrollTop:$("#"+q).offset().top-195},400)})}function d(){$("#navbar ul ul").css("display","none")}function f(){$("[active]").removeAttr("active"),$("[subactive]").removeAttr("subactive")}$(window).scroll(function(){var q=document.getElementById("navbar"),r=document.getElementById("navbarWrapper"),s=r.getBoundingClientRect().top;q.style.position=0>=s?"fixed":"static"});var g=["execSummary","problemStatement",{literature:["literatureData","literatureProcessing","literatureResults"]},{twitter:["twitterData","twitterProcessing","twitterResults"]},"conclusion","acknowlage","citations",{apendicies:["appendixA","appendixB","appendixC","appendixD","appendixE"]},"code"],h=[];var _iteratorNormalCompletion=!0,_didIteratorError=!1,_iteratorError=void 0;try{for(var _step,_iterator=g[Symbol.iterator]();!(_iteratorNormalCompletion=(_step=_iterator.next()).done);_iteratorNormalCompletion=!0){var p=_step.value;if("string"==typeof p)c(document.getElementById("ref_"+p),p),h.push({elem:document.getElementById(p),name:p,children:!1});else for(var q in p){for(var j=[],k=p[q].length,_r=0;_r<k;_r++)c(document.getElementById("ref_"+p[q][_r]),p[q][_r]),j.push({elem:document.getElementById(p[q][_r]),name:p[q][_r]});c(document.getElementById("ref_"+q),q),h.push({elem:document.getElementById(q),name:q,children:j})}}}catch(err){_didIteratorError=!0,_iteratorError=err}finally{try{!_iteratorNormalCompletion&&_iterator.return&&_iterator.return()}finally{if(_didIteratorError)throw _iteratorError}}console.log(h);var m=h.length,o=null;$(window).scroll(function(){var q=h[0];for(var s=1;s<m&&200>h[s].elem.getBoundingClientRect().top;s++)q=h[s];if(!1!=q.children){var r=q.children[0];for(var _s=1;_s<q.children.length&&200>q.children[_s].elem.getBoundingClientRect().top;_s++)r=q.children[_s];(null==o||o!=r)&&(d(),document.getElementById(q.name+"Foldout").style.display="block",f(),document.getElementById("ref_"+r.name).setAttribute("active",void 0),document.getElementById("ref_"+q.name).setAttribute("subactive",void 0),o=r)}else(null==o||o!=q)&&(d(),f(),document.getElementById("ref_"+q.name).setAttribute("active",void 0),o=q)}),window.setTimeout(function(){$(window).trigger("scroll")},10)}(),function(){$(".appendixRef").click(function(c){var d=$("#appendix"+$(c.target).attr("data-ref")),f=$("#appendix"+$(c.target).attr("data-ref")+" + h2");$("html, body").animate({scrollTop:d.offset().top-195},400),f.css({background:"#FFB133"}),window.setTimeout(function(){console.log(f),f.css({background:"#ffffff"})},800)})}(),function(){function c(){f.removeAttr("data-open"),window.setTimeout(function(){f.css({top:"-100%"})},400)}function d(m){f.css({top:"0%"}),f.attr("data-open",""),h.attr("src",m)}var f=$("#fullscreen"),g=document.getElementById("fullscreen"),h=$("img",f),j=$("#close"),k=$("#reportBody img.fig");f.click(function(m){m.target==g&&c()}),j.click(function(){c()}),k.click(function(m){console.log(m),d(m.target.getAttribute("src"))})}();
//# sourceMappingURL=main.js.map