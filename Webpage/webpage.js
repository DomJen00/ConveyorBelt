const textInput = document.getElementById("textInput");
const moveBtn = document.getElementById("moveBtn");
const accBtn = document.getElementById("accBtn");
const deccBtn = document.getElementById("deccBtn");
const stopBtn = document.getElementById("stopBtn");
const feedback = document.getElementById("status");

moveBtn.addEventListener("click", () => {
	let speed = parseInt(textInput.value);
	moveMotor(speed);
});

stopBtn.addEventListener("click", stopMotor);
accBtn.addEventListener("click", accMotor);
deccBtn.addEventListener("click", deccMotor);

function moveMotor(speed){
	if(isNaN(speed)) {
		alert("Please enter a valid speed!");
		return;		
	}
	
	if (Math.abs(speed) < 1000 || Math.abs(speed) > 2500) {
		alert("Please enter a value between 1000 and 2500 rpm.");
		return;
	}	
	
	fetch("http://192.168.7.2:5555", {
		method: "POST",
		headers: {
			"Content-Type": "text/plain"
		},
		body: JSON.stringify({ rpm: speed })
	})
	.then(response => response.text())
	.then(data => {
		feedback.innerHTML = "RPM sent: " + speed;	
	})
	.catch(error => {		
		feedback.innerHTML = "Error: " + error;		
	});
}

function stopMotor() {
	const speed = 0;
	
	fetch("http://192.168.7.2:5555", {
		method: "POST",
		headers: {
			"Content-Type": "text/plain"
		},
		body: JSON.stringify({ rpm: speed })
	})
	.then(response => response.text())
	.then(data => {
		feedback.innerHTML = "RPM sent: " + speed;	
	})
	.catch(error => {		
		feedback.innerHTML = "Error: " + error;		
	});
}

function accMotor() {
	let actSpeed = parseInt(textInput.value);
	let newSpeed = actSpeed + 100;
		
	if (Math.abs(newSpeed) > 2500) {
		newSpeed = 2500;
	}
	
	textInput.value = newSpeed;
	moveMotor(newSpeed);
}

function deccMotor() {
	let actSpeed = parseInt(textInput.value);
	let newSpeed = actSpeed - 100;
		
	if (Math.abs(newSpeed) < 1000) {
		newSpeed = 1000;
	}
	
	textInput.value = newSpeed;
	moveMotor(newSpeed);
}