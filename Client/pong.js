var width = 500;
var height = 500;
var keysInput = {};

var canvas = document.createElement('canvas');
canvas.id = 'canvas';
canvas.width = width;
canvas.height = height;
var context = canvas.getContext('2d');

var scoreDisplay = document.createTextNode("Test");

function Ball(x, y){
	this.x = x;
	this.y = y;
	this.radius = 5;
}

Ball.prototype.setPosition = function(x, y){
	this.x = x;
	this.y = y;
}

Ball.prototype.render = function(){
	context.beginPath();
	context.arc(this.x, this.y, this.radius, 2*Math.PI, false);
	context.fillStyle = "#FF3300";
	context.fill();
};

function Paddle(x, y, width, height){
	this.x = x;
	this.y = y;
	this.width = width;
	this.height = height;
}

Paddle.prototype.render = function(){
	context.fillStyle = "#33FF33";
	context.fillRect(this.x, this.y, this.width, this.height);
};

Paddle.prototype.move = function(movement){
	this.x += movement;
};

function Player(){
	this.paddle = new Paddle(200, 490, 100, 5);
}

Player.prototype.getX = function(){
	return this.paddle.x;
}

Player.prototype.getY = function(){
	return this.paddle.y;
}

Player.prototype.render = function(){
	this.paddle.render();
};

Player.prototype.update = function(){
	for(var key in keysInput){
		var value = Number(key);
		if(value == 37)			//Left Arrow
			this.paddle.move(-5);
		else if(value == 39)	//Right Arrow
			this.paddle.move(5);
		else
			this.paddle.move(0);
	}
};

function Score(){
	this.name = "";
	this.score = 0;
	this.print = true;
}

Score.prototype.setName = function(name){
	this.name = name;
}

Score.prototype.getName = function(){
	return this.name;
}

Score.prototype.setScore = function(score){
	this.score = score;
	this.print = true;
};

Score.prototype.render = function(msg){
	if(this.print){
		try{
			document.body.removeChild(scoreSection);
		}catch(error){}
		
		var scoreSection = document.createElement("P");
		var scoreDisplay = document.createTextNode(msg);
		scoreSection.appendChild(scoreDisplay);
		document.body.appendChild(scoreSection);
		
		this.print = false;
	}
};

var animate = window.requestAnimationFrame ||
			  window.webkitRequestAnimationFrame ||
			  window.mozRequestAnimationFrame ||
			  function(callback){
				  window.setTimeout(callback, 1000/60)
			  };

window.addEventListener("keyDown", function(event){ keysInput[event.keyCode] = true; });
window.addEventListener("keyUP", function(event){ delete keysInput[event.keyCode]; });
