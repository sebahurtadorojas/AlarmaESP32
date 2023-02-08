window.addEventListener('load',getReadings);
const elemEstado=document.getElementById("check");
elemEstado.addEventListener("click",cambiarEstado);

function cambiarEstado(){
    console.log(elemEstado.checked);
}

function getReadings(){
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);
      var temp = myObj.temperature;
      var hum = myObj.humidity;
      gaugeTemp.value = temp;
      gaugeHum.value = hum;
    }
  }; 
  xhr.open("GET", "/readings", true);
  xhr.send();
}


function consultaGET(consulta){
    const Http = new XMLHttpRequest();
    Http.open("GET",consulta);
    Http.send();
    Http.onreadystatechange = (e) => {
        console.log(Http.status);
        console.log(Http.responseText);
    }
}
