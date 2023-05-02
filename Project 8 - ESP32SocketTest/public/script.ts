const url: string = "ws://192.168.1.144:1337/";
var output: HTMLElement | null;
var button: HTMLElement | null;
var canvas: HTMLCanvasElement | null;
var context: CanvasRenderingContext2D | null;
var websocket: WebSocket;

function init(): void {
    button = document.getElementById("toggleButton");
    output = document.getElementById("output");
    canvas = document.getElementById("led") as HTMLCanvasElement;

    context = canvas.getContext("2d");
    if(context) {
        context.arc(25, 25, 15, 0, Math.PI * 2, false);
        context.lineWidth = 3;
        context.strokeStyle = "black";
        context.stroke();
        context.fillStyle = "black";
        context.fill();
    
        wsConnect(url);
    }
}

function doSend(message: string): void {
    console.log("Sending: " + message);
    websocket.send(message);
}

function wsConnect(url: string): void {
    websocket = new WebSocket(url);
    
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
    websocket.onerror = onError;
}

const onOpen = (event: Event): void  => {
    console.log("Connected");
    doSend("getLEDState");
}

const onClose = (event: CloseEvent): void => {
    console.log("Disconnected");
    setTimeout(() => {wsConnect(url)}, 2000);
}

const onMessage = (event: MessageEvent): void  => {
    console.log("Received: " +  event.data);
    
    switch(event.data) {
        case "0":
            if (context) {
                console.log("LED is off");
                context.fillStyle = "black";
                context?.fill();
            }
            break;
        case "1": 
            if (context) {
                console.log("LED is on");
                context.fillStyle = "red";
                context?.fill();
            }
            break;
        default: 
            break;
    }
}

const onError = (event: Event): void => {
    console.error("ERROR: " + event);
}

function onPress() {
    doSend("toggleLED");
    doSend("getLEDState");
}

window.addEventListener("load", init, false);