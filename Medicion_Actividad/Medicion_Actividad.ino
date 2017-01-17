int x;
int actividad=0;
char act;

void setup(){

}

void loop(){
	for (i=0; i<100; i++){
		x=analogRead(A0)+analogRead(A3)+analogRead(A2);
		actividad=actividad+x;
		delay(100);
	}
	if (actividad>=300000){
		act="alta";
	}
	if (actividad<=300000 || actividad>=200000){
		act="media";
	}
	if (activida<=200000){
		act="baja";
	}
	return act;
}