var width = 500;
var height = 500;
var keysInput = {};

var canvas = document.createElement('canvas');
canvas.id = 'canvas';
canvas.width = width;
canvas.height = height;
var context = canvas.getContext('2d');

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
	if(movement!=0){
		this.x += movement;
		if(this.x < 0)
			this.x = 0;
		else if(this.x+this.width > width)
			this.x = width - this.width;
	}
};

function Player(){
	this.paddle = new Paddle(200, 490, 100, 5);
	this.speed = 2;
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
			this.paddle.move(-this.speed);
		else if(value == 39)	//Right Arrow
			this.paddle.move(this.speed);
		else
			this.paddle.move(0);
	}
};

function Score(){
	this.name = "";
	this.score = 0;
}

Score.prototype.setName = function(name){
	this.name = name;
}

Score.prototype.getName = function(){
	return this.name;
}

Score.prototype.setScore = function(score){
	this.score = score;
};

Score.prototype.render = function(){
	document.getElementById('score').innerHTML = this.name + "'s Score: " + this.score;
};

var animate = window.requestAnimationFrame ||
			  window.webkitRequestAnimationFrame ||
			  window.mozRequestAnimationFrame ||
			  function(callback){
				  window.setTimeout(callback, 1000/60)
			  };

window.addEventListener("keydown", function(event){ keysInput[event.keyCode] = true; });
window.addEventListener("keyup", function(event){ delete keysInput[event.keyCode]; });
