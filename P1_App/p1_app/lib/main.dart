import 'package:flutter/material.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';


void main() {
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  MyAppState createState() => MyAppState();
}

class MyAppState extends State<MyApp> {
  late MqttServerClient client;
  String fsrData = "No alert";

  @override
  void initState() {
    super.initState();
    setupMqttClient();
  }

  void setupMqttClient() async {
    client = MqttServerClient('3.15.24.121', 'FlutterClient');
    client.port = 1883;
    client.keepAlivePeriod = 20;
    client.onConnected = onConnected;
    client.onDisconnected = onDisconnected;
    client.onSubscribed = onSubscribed;
    client.logging(on: true);

    try {
      await client.connect();
    } catch (e) {
      print('Failed to connect: $e');
    }
  }

  void onConnected() {
    print('Connected to MQTT broker');
    client.subscribe('fsr/alert', MqttQos.atMostOnce);
  }

  void onDisconnected() {
    print('Disconnected from MQTT broker');
  }

  void onSubscribed(String topic) {
    print('Subscribed to topic: $topic');
    client.updates!.listen((List<MqttReceivedMessage<MqttMessage>>? event) {
      final recMess = event![0].payload as MqttPublishMessage;
      final payload = MqttPublishPayload.bytesToStringAsString(recMess.payload.message);
      setState(() {
        fsrData = "FSR Alert: $payload"; // Update with the FSR value received
      });
      showNotification(payload); // Show notification on threshold alert
    });
  }

  void showNotification(String data) {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: Text("FSR Alert"),
          content: Text("Threshold reached: $data"),
          actions: <Widget>[
            TextButton(
              onPressed: () {
                Navigator.of(context).pop();
              },
              child: Text("OK"),
            ),
          ],
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: Text("FSR Notifier App"),
        ),
        body: Center(
          child: Text(fsrData),
        ),
      ),
    );
  }
}
