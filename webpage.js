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

stopBtn.addEventListener("click", () => {
	moveMotor(0);
});

accBtn.addEventListener("click", accMotor);
deccBtn.addEventListener("click", deccMotor);

function moveMotor(speed) {
	if (isNaN(speed)) {
		alert("Please enter a valid number!");
		return;
	}

	if (speed !== 0 && (Math.abs(speed) < 1000 || Math.abs(speed) > 2500)) {
		alert("Please enter a value between ±1000 and ±2500 rpm.");
		return;
	}

	fetch("/set-speed", {
		method: "POST",
		headers: {
			"Content-Type": "text/plain"
		},
		body: speed.toString()
	})
	.then(response => {
		if (!response.ok) throw new Error("HTTP error " + response.status);
		return response.text();
	})
	.then(txt => {
		feedback.innerText = txt;
	})
	.catch(error => {
		feedback.innerText = "Error: " + error;
	});
}

function accMotor() {
	let actSpeed = parseInt(textInput.value);
	if (isNaN(actSpeed)) {
		alert("Please enter a valid number!");
		return;
	}

	let dir = Math.sign(actSpeed) || 1;
	let speedAbs = Math.abs(actSpeed);
	let newSpeed = speedAbs + 100;

	if (newSpeed > 2500) {
		newSpeed = 2500;
	}
	
	textInput.value = newSpeed * dir;
	moveMotor(newSpeed * dir);
}

function deccMotor() {
	let actSpeed = parseInt(textInput.value);
	if (isNaN(actSpeed)) {
		alert("Please enter a valid number!");
		return;
	}

	let dir = Math.sign(actSpeed) || 1;
	let speedAbs = Math.abs(actSpeed);
	let newSpeed = speedAbs - 100;

	if (newSpeed < 1000 && newSpeed !== 0) {
		newSpeed = 1000;	
	} 
	
	textInput.value = newSpeed * dir;
	moveMotor(newSpeed * dir);
}