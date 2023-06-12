#include <Ps3Controller.h>

//int player = 0;
//int battery = 0;
//
//void notify()
//{
//    //--- Digital cross/square/triangle/circle button events ---
//    if( Ps3.event.button_down.cross )
//        Serial.println("Started pressing the cross button");
//    if( Ps3.event.button_up.cross )
//        Serial.println("Released the cross button");
//
//    if( Ps3.event.button_down.square )
//        Serial.println("Started pressing the square button");
//    if( Ps3.event.button_up.square )
//        Serial.println("Released the square button");
//
//    if( Ps3.event.button_down.triangle )
//        Serial.println("Started pressing the triangle button");
//    if( Ps3.event.button_up.triangle )
//        Serial.println("Released the triangle button");
//
//    if( Ps3.event.button_down.circle )
//        Serial.println("Started pressing the circle button");
//    if( Ps3.event.button_up.circle )
//        Serial.println("Released the circle button");
//
//    //--------------- Digital D-pad button events --------------
//    if( Ps3.event.button_down.up )
//        Serial.println("Started pressing the up button");
//    if( Ps3.event.button_up.up )
//        Serial.println("Released the up button");
//
//    if( Ps3.event.button_down.right )
//        Serial.println("Started pressing the right button");
//    if( Ps3.event.button_up.right )
//        Serial.println("Released the right button");
//
//    if( Ps3.event.button_down.down )
//        Serial.println("Started pressing the down button");
//    if( Ps3.event.button_up.down )
//        Serial.println("Released the down button");
//
//    if( Ps3.event.button_down.left )
//        Serial.println("Started pressing the left button");
//    if( Ps3.event.button_up.left )
//        Serial.println("Released the left button");
//
//    //------------- Digital shoulder button events -------------
//    if( Ps3.event.button_down.l1 )
//        Serial.println("Started pressing the left shoulder button");
//    if( Ps3.event.button_up.l1 )
//        Serial.println("Released the left shoulder button");
//
//    if( Ps3.event.button_down.r1 )
//        Serial.println("Started pressing the right shoulder button");
//    if( Ps3.event.button_up.r1 )
//        Serial.println("Released the right shoulder button");
//
//    //-------------- Digital trigger button events -------------
//    if( Ps3.event.button_down.l2 )
//        Serial.println("Started pressing the left trigger button");
//    if( Ps3.event.button_up.l2 )
//        Serial.println("Released the left trigger button");
//
//    if( Ps3.event.button_down.r2 )
//        Serial.println("Started pressing the right trigger button");
//    if( Ps3.event.button_up.r2 )
//        Serial.println("Released the right trigger button");
//
//    //--------------- Digital stick button events --------------
//    if( Ps3.event.button_down.l3 )
//        Serial.println("Started pressing the left stick button");
//    if( Ps3.event.button_up.l3 )
//        Serial.println("Released the left stick button");
//
//    if( Ps3.event.button_down.r3 )
//        Serial.println("Started pressing the right stick button");
//    if( Ps3.event.button_up.r3 )
//        Serial.println("Released the right stick button");
//
//    //---------- Digital select/start/ps button events ---------
//    if( Ps3.event.button_down.select )
//        Serial.println("Started pressing the select button");
//    if( Ps3.event.button_up.select )
//        Serial.println("Released the select button");
//
//    if( Ps3.event.button_down.start )
//        Serial.println("Started pressing the start button");
//    if( Ps3.event.button_up.start )
//        Serial.println("Released the start button");
//
//    if( Ps3.event.button_down.ps )
//        Serial.println("Started pressing the Playstation button");
//    if( Ps3.event.button_up.ps )
//        Serial.println("Released the Playstation button");
//
//
//    //---------------- Analog stick value events ---------------
//   if( abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2 ){
//       Serial.print("Moved the left stick:");
//       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
//       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
//       Serial.println();
//    }
//
//   if( abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2 ){
//       Serial.print("Moved the right stick:");
//       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.rx, DEC);
//       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ry, DEC);
//       Serial.println();
//   }
//
//   //--------------- Analog D-pad button events ----------------
//   if( abs(Ps3.event.analog_changed.button.up) ){
//       Serial.print("Pressing the up button: ");
//       Serial.println(Ps3.data.analog.button.up, DEC);
//   }
//
//   if( abs(Ps3.event.analog_changed.button.right) ){
//       Serial.print("Pressing the right button: ");
//       Serial.println(Ps3.data.analog.button.right, DEC);
//   }
//
//   if( abs(Ps3.event.analog_changed.button.down) ){
//       Serial.print("Pressing the down button: ");
//       Serial.println(Ps3.data.analog.button.down, DEC);
//   }
//
//   if( abs(Ps3.event.analog_changed.button.left) ){
//       Serial.print("Pressing the left button: ");
//       Serial.println(Ps3.data.analog.button.left, DEC);
//   }
//
//   //---------- Analog shoulder/trigger button events ----------
//   if( abs(Ps3.event.analog_changed.button.l1)){
//       Serial.print("Pressing the left shoulder button: ");
//       Serial.println(Ps3.data.analog.button.l1, DEC);
//   }
//
//   if( abs(Ps3.event.analog_changed.button.r1) ){
//       Serial.print("Pressing the right shoulder button: ");
//       Serial.println(Ps3.data.analog.button.r1, DEC);
//   }
//
//   if( abs(Ps3.event.analog_changed.button.l2) ){
//       Serial.print("Pressing the left trigger button: ");
//       Serial.println(Ps3.data.analog.button.l2, DEC);
//   }
//
//   if( abs(Ps3.event.analog_changed.button.r2) ){
//       Serial.print("Pressing the right trigger button: ");
//       Serial.println(Ps3.data.analog.button.r2, DEC);
//   }
//
//   //---- Analog cross/square/triangle/circle button events ----
//   if( abs(Ps3.event.analog_changed.button.triangle)){
//       Serial.print("Pressing the triangle button: ");
//       Serial.println(Ps3.data.analog.button.triangle, DEC);
//   }
//
//   if( abs(Ps3.event.analog_changed.button.circle) ){
//       Serial.print("Pressing the circle button: ");
//       Serial.println(Ps3.data.analog.button.circle, DEC);
//   }
//
//   if( abs(Ps3.event.analog_changed.button.cross) ){
//       Serial.print("Pressing the cross button: ");
//       Serial.println(Ps3.data.analog.button.cross, DEC);
//   }
//
//   if( abs(Ps3.event.analog_changed.button.square) ){
//       Serial.print("Pressing the square button: ");
//       Serial.println(Ps3.data.analog.button.square, DEC);
//   }
//
//   //---------------------- Battery events ---------------------
//    if( battery != Ps3.data.status.battery ){
//        battery = Ps3.data.status.battery;
//        Serial.print("The controller battery is ");
//        if( battery == ps3_status_battery_charging )      Serial.println("charging");
//        else if( battery == ps3_status_battery_full )     Serial.println("FULL");
//        else if( battery == ps3_status_battery_high )     Serial.println("HIGH");
//        else if( battery == ps3_status_battery_low)       Serial.println("LOW");
//        else if( battery == ps3_status_battery_dying )    Serial.println("DYING");
//        else if( battery == ps3_status_battery_shutdown ) Serial.println("SHUTDOWN");
//        else Serial.println("UNDEFINED");
//    }
//
//}

/*****マクロ定義*****/
#define ID_STOP 1
#define ID_NORMAL 2
#define ID_MTORIGIN 3

/*****Global変数定義*****/
volatile unsigned char u1g_event_xcircle;
volatile unsigned char u1g_event_xcross;
volatile unsigned char u1g_event_xtriangle;
volatile unsigned char u1g_event_idmode = ID_STOP;

void autocar_cntget()
{
  if ( Ps3.event.button_down.start )  { Serial.write(255); }
  if ( Ps3.event.button_down.circle ) { u1g_event_idmode = ID_NORMAL; }
  if ( Ps3.event.button_down.cross ) { u1g_event_idmode = ID_STOP; }
  if ( Ps3.event.button_down.triangle ) { u1g_event_idmode = ID_MTORIGIN; }
}

void setup()
{
    Serial.begin(115200);
    Ps3.attach(autocar_cntget);
    Ps3.begin("FC:F5:C4:45:75:8E");
}

void loop()
{
  while(Ps3.isConnected())
  {
    while(1)
    {
      while(Serial.available())
      {
        delay(100);
        Serial.write(Ps3.data.analog.stick.ly+128);
        Serial.write(Ps3.data.analog.stick.rx+128);
        Serial.write(u1g_event_idmode);
        switch ( Ps3.data.status.battery )
        {
          case ps3_status_battery_full:
          Ps3.setPlayer(4);
          break;
          case ps3_status_battery_high:
          Ps3.setPlayer(3);
          break;
          case ps3_status_battery_low:
          Ps3.setPlayer(2);
          break;
          case ps3_status_battery_dying:
          Ps3.setPlayer(1);
          break;
        }
      }
    }
  }
}
