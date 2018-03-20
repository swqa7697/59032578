var width = 500;
var height = 500;
var keysInput = {};

var canvas = document.createElement('canvas');
canvas.id = 'canvas';
canvas.width = width;
canvas.height = height;
var context = canvas.getContext('2d');

function Ball(x, y){
	this.radius = 5;
	this.x = x;
	this.y = y;
	this.x_speed = 0;
	this.y_speed = 0;
}

//Deprecated function
/*
Ball.prototype.setPosition = function(x, y){
	this.x = x;
	this.y = y;
};
*/

Ball.prototype.setMovement = function(x, y){
	this.x_speed = (x - this.x)/6;
	this.y_speed = (y - this.y)/6;
};

Ball.prototype.update = function(){
	this.x += this.x_speed;
	this.y += this.y_speed;
};

Ball.prototype.render = function(){
	context.beginPath();
	context.arc(this.x, this.y, this.radius, 2*Math.PI, false);
	context.fillStyle = "#FF9900";
	context.fill();
};

function Paddle(x, y, width, height){
	this.x = x;
	this.y = y;
	this.width = width;
	this.height = height;
}

Paddle.prototype.render = function(){
	context.fillStyle = "#000000";
	context.fillRect(this.x, this.y, this.width, this.height);
};

Paddle.prototype.move = function(movementX, movementY){
	if(movementX != 0){
		this.x += movementX;
		if(this.x < 0)
			this.x = 0;
		else if(this.x+this.width > width)
			this.x = width - this.width;
	}
	if(movementY != 0){
		this.y += movementY;
		if(this.y < 0)
			this.y = 0;
		else if(this.y+this.height > height)
			this.y = height - this.height;
	}
};

function Player(ID){
	if(ID == 0)
		this.paddle = new Paddle(200, 490, 100, 5);
	else if(ID == 1)
		this.paddle = new Paddle(490, 200, 5, 100);
	else if(ID == 2)
		this.paddle = new Paddle(200, 5, 100, 5);
	else if(ID == 3)
		this.paddle = new Paddle(5, 200, 5, 100);
	else
		throw "Illegal player";
	
	this.speed = 2;
	this.player = ID;
	this.movement = 0;
	this.stepAhead = 0;
}

Player.prototype.getX = function(){
	return this.paddle.x;
};

Player.prototype.getY = function(){
	return this.paddle.y;
};

Player.prototype.setX = function(X){
	this.paddle.x = X;
};

Player.prototype.setY = function(Y){
	this.paddle.y = Y;
};

Player.prototype.setMovement = function(des){
	if(this.player == 0 || this.player == 2)
		this.movement = (des - this.paddle.x)/6;
	else
		this.movement = (des - this.paddle.y)/6;
	
	this.stepAhead = 6;
};

Player.prototype.updateMovement = function(){
	if(this.stepAhead > 0){
		if(this.player == 0 || this.player == 2)
			this.paddle.x += this.movement;
		else
			this.paddle.y += this.movement;
		
		this.stepAhead--;
	}
};

Player.prototype.update = function(){
	if(this.player == 0 || this.player == 2){
		for(var key in keysInput){
			var value = Number(key);
			if(value == 37)			//Left Arrow
				this.paddle.move(-this.speed, 0);
			else if(value == 39)	//Right Arrow
				this.paddle.move(this.speed, 0);
			else
				this.paddle.move(0, 0);
		}
	}
	else{
		for(var key in keysInput){
			var value = Number(key);
			if(value == 38)			//Up Arrow
				this.paddle.move(0, -this.speed);
			else if(value == 40)	//Down Arrow
				this.paddle.move(0, this.speed);
			else
				this.paddle.move(0, 0);
		}
	}
};

Player.prototype.render = function(){
	this.paddle.render();
};

function Score(){
	this.name = "";
	this.score = 0;
}

Score.prototype.setName = function(name){
	this.name = name;
};

Score.prototype.getName = function(){
	return this.name;
};

Score.prototype.setScore = function(score){
	this.score = score;
};

Score.prototype.print = function(elementID){
	document.getElementById(elementID).innerHTML = this.name + "'s Score: " + this.score;
};

var animate = window.requestAnimationFrame ||
			  window.webkitRequestAnimationFrame ||
			  window.mozRequestAnimationFrame ||
			  function(callback){
				  window.setTimeout(callback, 1000/60)
			  };

window.addEventListener("keydown", function(event){ keysInput[event.keyCode] = true; });
window.addEventListener("keyup", function(event){ delete keysInput[event.keyCode]; });
