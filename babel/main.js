"use strict";
// Header Scroll
(function(){
	// Init scroll controller
	var controller = new ScrollMagic.Controller();
	// Animation Tweens
	var tweenFadeIn = (id) => new TweenMax.to(id, 1.5, {
	    opacity: 1
	});
	var tweenFadeOut = (id) => new TweenMax.to(id, 1.5, {
	    opacity: 0
	});
	// Header
	new ScrollMagic.Scene({
			duration: 100,    // the scene should last for a scroll distance of 100px
			offset: 0      // start this scene after scrolling for 50px
		})
		//.addIndicators()
		.setPin("#headerWrapper") // pins the element for the the scene's duration
		.addTo(controller);
	new ScrollMagic.Scene({
			duration: 50,
			offset: 10
		})
		.setTween(tweenFadeOut("#instructions"))
		//.addIndicators()
		.addTo(controller);
	new ScrollMagic.Scene({
			duration: 200,
			offset: 100
		})
		.setTween(tweenFadeOut("#headerWrapper"))
		//.addIndicators()
		.setPin("#headerWrapper")
		.addTo(controller);
})();
// Sidebar
(function() {
	// Pin sidebar
	$(window).scroll((e) => {
		// Optimize?
		var el = document.getElementById("navbar"),
			elWrapper = document.getElementById("navbarWrapper");
		var elTop = elWrapper.getBoundingClientRect().top;
		if(elTop <= 0) {
			el.style.position = "fixed";
		} else {
			el.style.position = "static";
		}
	});
	
	// Navbar selection
	var tableOfContents = [
			"execSummary",
			"problemStatement",
			{
				"literature": [
					"literatureData",
					"literatureProcessing",
					"literatureResults"
				]
			},
			{
				"twitter": [
					"twitterData",
					"twitterProcessing",
					"twitterResults"
				]
			},
			"conclusion",
			"acknowlage",
			"citations",
			{
				"apendicies": [
					"appendixA",
					"appendixB",
					"appendixC",
					"appendixD",
					"appendixE"
				]
			},
			"code"
		],
		dom = []; // {elem: E, children: false}
	function navigationInit(e, n){
		$(e).click(()=>{
			$('html, body').animate({
				scrollTop: $("#" + n).offset().top - 195
			}, 400);
		});
	}
	for(let a of tableOfContents){
		if(typeof a == "string") {
			navigationInit(document.getElementById("ref_" + a), a);
			dom.push({
				elem: document.getElementById(a),
				name: a,
				children: false
			});
		} else {
			for(let b in a){
				var kids = [];
				var l = a[b].length;
				for(let i = 0; i < l; i++){
					navigationInit(document.getElementById("ref_" + a[b][i]), a[b][i]);
					kids.push({
						elem: document.getElementById(a[b][i]),
						name: a[b][i]
					});
				}
				navigationInit(document.getElementById("ref_" + b), b);
				dom.push({
					elem: document.getElementById(b),
					name: b,
					children: kids
				});
			}
		}
	}
	console.log(dom);
	function collapseAll(){
		$("#navbar ul ul").css("display", "none");
	}
	function deselect(){
		$("[active]").removeAttr("active");
		$("[subactive]").removeAttr("subactive");
	}
	let LEN = dom.length;
	var lastSelected = null;
	$(window).scroll((e) => {
		let onScreen = dom[0];
		for(let i = 1; i < LEN; i++){
			if(dom[i].elem.getBoundingClientRect().top < 200){
				onScreen = dom[i];
			} else {
				break;
			}
		}
		if(onScreen.children != false) {
			var subScreen = onScreen.children[0];
			for(let i = 1; i < onScreen.children.length; i++){
				if(onScreen.children[i].elem.getBoundingClientRect().top < 200){
					subScreen = onScreen.children[i];
				} else {
					break;
				}
			}
			if(lastSelected == null || lastSelected != subScreen){
				collapseAll();
				document.getElementById(onScreen.name + "Foldout").style.display = "block";
				deselect();
				// activate
				document.getElementById("ref_" + subScreen.name).setAttribute("active", undefined);
				document.getElementById("ref_" + onScreen.name).setAttribute("subactive", undefined);
				lastSelected = subScreen;
			}
		} else {
			if(lastSelected == null || lastSelected != onScreen){
				collapseAll();
				deselect();
				// activate
				document.getElementById("ref_" + onScreen.name).setAttribute("active", undefined);
				
				lastSelected = onScreen;
			}
		}
	});
	
	// Trigger Scroll event for init event methods
	window.setTimeout(() => {
		$(window).trigger("scroll");
	}, 10);
})();
// Appendicies
(function(){
	$(".appendixRef").click((e)=>{
		let to = $("#appendix" + $(e.target).attr("data-ref")),
			sibling =  $("#appendix" + $(e.target).attr("data-ref") + " + h2");
		$('html, body').animate({
			scrollTop: to.offset().top - 195
		}, 400);
		sibling.css({
			background: "#FFB133"
		});
		window.setTimeout(()=>{
			console.log(sibling);
			sibling.css({
				background: "#ffffff"
			});
		}, 800);
	});
})();
// Images
(function() {
	let fullscreen = $("#fullscreen"),
		fe = document.getElementById("fullscreen"), // iron!
		img = $("img", fullscreen),
		close = $("#close"),
		imgs = $("#reportBody img.fig");
	function closeFs(){
		fullscreen.removeAttr("data-open");
		window.setTimeout(() => {
			fullscreen.css({
				"top": "-100%"
			});
		}, 400);
	}
	function openFs(src){
		fullscreen.css({
			"top": "0%"
		});
		fullscreen.attr("data-open", "");
		img.attr("src", src);
	}
	fullscreen.click((e) => {
		if (e.target == fe)
			closeFs();
	});
	close.click((e) => {
		closeFs();
	});
	imgs.click((e) => {
		console.log(e);
		openFs(e.target.getAttribute("src"));
	});
})();
