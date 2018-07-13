
//	a simple custom control with basic communication with the sample unreal project

//	a place for some globals
var app = {
	
};

//	grab a reference to the canvas and draw something on it
function setUpCanvas()
{
	app.canvasElement = document.getElementById("canvas");
	var ctx = app.canvasElement.getContext('2d');
	ctx.strokeStyle = "#FF0000";
	ctx.lineWidth = 8;
	ctx.strokeRect(0, 0, app.canvasElement.width, app.canvasElement.height);
}

//	for testing, output debug text to screen
function screenLog(txt)
{
	var outBox = document.getElementById('debug');
	outBox.innerHTML += txt + "\n";
};

//	set up our stream position handler so we can position our overlay correctly
function setUpPositionHandler()
{
	//	our stream position handler
	var mixerPositionHandler = function(position) {
		const player = position.connectedPlayer;
		const canvasElement = app.canvasElement;
		
		var x = player.left;
		var y = player.top;
		var w = player.width;
		var h = player.height;

		//	some debug output to track what values we're getting
		console.log("mixer position handler");
		console.log("  position:");
		console.log("  " + x + "," + y + "  " + w + " x " + h);
		
		//	fix the h value we're using here to keep our aspect ratio the same but match player width,
		//	in case player is not at our same aspect ratio, which it usually is.
		h = w * (canvasElement.height / canvasElement.width);
		
		//	now position our canvas relative to player space, using style properties.
		//	note: actual canvas size doesn't change, just css scaling
		x = x | 0;
		y = y | 0;
		w = w | 0;
		h = h | 0;
		canvasElement.style.position = "fixed";
		canvasElement.style.left = '' + x + 'px';
		canvasElement.style.top = '' + y + 'px';
		canvasElement.style.width = '' + w + 'px';
		canvasElement.style.height = '' + h + 'px';
		
		//	remember how much that's been scaled
		app.uiScale = canvasElement.height / h;
		console.log("ui scale " + app.uiScale);

		console.log("canvas style set to " + x + "," + y + "  " + w + " x " + h);
	};
	//	now actually hook that up
	mixer.display.position().subscribe(mixerPositionHandler);
}

//	Set up handlers for the various messages we care about.
function setUpMessages()
{
	console.log("registering socket callbacks...");
	mixer.socket.on('event', function(message) {
		//	just echo whatever we got
		var txt = "*** EVENT: " + message.code;
		if (message.variant)
			txt += " + " + message.variant;
		screenLog(txt);
		
		//console.log(message);
	});
	
	mixer.socket.on('onSceneUpdate', function(message){
		screenLog("*** scene update");
	});

	mixer.socket.on('onParticipantUpdate', function(message){
		var pe = message.participants[0];
		
		screenLog("*** PARTICIPANT update: -> clicks from me: " + pe.clicks);
		
		//console.log(pe);
	});
	
	mixer.socket.on('onParticipantJoin', function(message){
		screenLog("*** participant join");
	});
				 
	mixer.socket.on('onControlUpdate', function(message) {
		screenLog("*** CONTROL update");
		var controlList = message.controls;
		console.log("  " + controlList.length);
		for (var i = 0; i < controlList.length; i++)
		{
			var c = controlList[i];
			console.log("  id : " + c.controlID);
			if (c.controlID === 'anchor')
			{
				if (c.numGlobalClicks)	//	if there's anything stored there.
				{
					screenLog("       -> num global clicks: " + c.numGlobalClicks);
				}
			} else if (c.controlID === 'my_first_button')
			{
				console.log("****************************  GRR! OVERWRITTEN DEFAULT CONTROLS");
			}
		}
	});

}

//	Set up our own input handling for getting clicks
function setUpInput()
{
	// Whenever someone clicks on "Hello World", we'll send an event
	// to the game client on the control ID "anchor"
	document.getElementById('hello-world').onclick = function(event) {
		console.log("sending button click message");
		mixer.socket.call('giveInput', {
			controlID: 'anchor',	//	this is needed to identify which control to use to send the message
			event: 'click',	//	and some value here is expected on the GC side, but in our case ignored
			//	and the rest here can be whatever we want it to be.
			message : {
				code : 'buttonclick',
			}
		});
	};
	
	//	When someone clicks in the canvas, send a position event
	app.canvasElement.addEventListener("mousedown", function(event) {
		var x = event.offsetX;
		var y = event.offsetY;
		console.log("got mouse down " + x + ", " + y);
		
		//	clicks are in scaled space,
		//	so convert back to what we think is our target screen space
		var overlayX = x * app.uiScale;
		var overlayY = y * app.uiScale;
		
		screenLog("<- sending sclick @ " + overlayX + ", " + overlayY);
		
		mixer.socket.call('giveInput', {
			controlID: 'anchor',	//	this is needed to identify which control to use to send the message
			event: 'message',	//	and some value here is expected on the GC side, but in our case ignored
			//	and the rest here can be whatever we want it to be.
			message : {
				code : 'sclick',
				x : overlayX,
				y : overlayY,
			}
			
		});
		
	});
}

//	put the video where we want it.
function positionVideo()
{
	// Move the video by a small offset amount
	const offset = 10 + Math.random() * 10;
	mixer.display.moveVideo({
		top: offset,
		bottom: offset,
		left: 0,
		right: offset,
	});
}

//	When we're done loading, set everything up
window.addEventListener('load', function initMixer()
{	
	setUpCanvas();
	setUpPositionHandler();
	setUpMessages();
	setUpInput();
	
	mixer.isLoaded();
	
	positionVideo();
});
