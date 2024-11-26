import 'package:flutter/material.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_browser_client.dart';
import 'dart:math';

List<List<String>> otpRecords = [];

void main() {
  runApp(const ParcelShieldApp());
}

String generateOtp() {
  Random random = Random();
  String otp = '';
  for (int i = 0; i < 8; i++) {
    otp += random.nextInt(10).toString(); // Generates a random digit (0-9)
  }
  return otp;
}

class ParcelShieldApp extends StatelessWidget {
  const ParcelShieldApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Parcel Shield',
      theme: ThemeData(
        primarySwatch: Colors.blue,
        scaffoldBackgroundColor: const Color(0xFFF3F4F6),
      ),
      home: const ParcelShieldHomePage(),
      debugShowCheckedModeBanner: false,
    );
  }
}

class ParcelShieldHomePage extends StatefulWidget {
  const ParcelShieldHomePage({Key? key}) : super(key: key);

  @override
  _ParcelShieldHomePageState createState() => _ParcelShieldHomePageState();
}

class _ParcelShieldHomePageState extends State<ParcelShieldHomePage> {
  late MqttBrowserClient client;
  String _otp = '';
  String _name = '';
  bool _isLoading = false;
  bool _hasPackage = false;
  String status = "Disconnected";

  @override
  void initState() {
    super.initState();
    setupMQTT();
  }

  void setupMQTT() async {
    client = MqttBrowserClient('ws://ec2-3-15-24-121.us-east-2.compute.amazonaws.com', 'flutter_client');
    client.port = 8081;
    client.logging(on: true);
    client.keepAlivePeriod = 20;

    client.onConnected = () {
      print("Connected to the MQTT broker!");
      setState(() {
        status = "Connected";
      });
      subscribeToPackageStatus();
    };

    client.onDisconnected = () {
      print("Disconnected from the MQTT broker.");
      setState(() {
        status = "Disconnected";
      });
    };

    try {
      await client.connect();
    } catch (e) {
      print("Connection error: $e");
      setState(() {
        status = "Error: $e";
      });
    }
  }

  void subscribeToPackageStatus() {
    client.subscribe('alerts/fsr', MqttQos.atMostOnce);
    client.updates!.listen((List<MqttReceivedMessage> messages) {
      final message = messages[0].payload as MqttPublishMessage;
      final payload = MqttPublishPayload.bytesToStringAsString(message.payload.message);

      print("Received package status: $payload");
      setState(() {
        _hasPackage = payload.toLowerCase() == 'present';
      });
    });
  }

  void publishOtp(String otp) {
    if (client.connectionStatus!.state == MqttConnectionState.connected) {
      final builder = MqttClientPayloadBuilder();
      builder.addString(otp);
      client.publishMessage('opt/request', MqttQos.atMostOnce, builder.payload!);
      print("OTP published: $otp");
    } else {
      print("Client not connected, cannot send OTP.");
    }
  }

  void _requestOtp() {
    if (_name.isEmpty) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(
          content: Text("Please enter a name before generating an OTP."),
          backgroundColor: Colors.red,
        ),
      );
      return;
    }
    else if (_name.length > 25 || _name.length < 3) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(
          content: Text("Please enter a name that is less than 25 characters and more than 3."),
          backgroundColor: Colors.red,
        ),
      );
      return;
    }
    else { // If the OTP name was valid, proceed
      setState(() {
        _isLoading = true;
      });

      Future.delayed(const Duration(seconds: 2), () {
        setState(() {
          _otp = generateOtp();
          _isLoading = false;
        });

        // Add the OTP and name to the global 2D array
        otpRecords.add([_name, _otp]);
        print("OTP Records: $otpRecords"); // For debugging
      });
    }
  }

  @override
  void dispose() {
    client.disconnect();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Parcel Shield'),
        centerTitle: true,
        backgroundColor: Colors.blueAccent,
        elevation: 0,
      ),
      body: Center(
        child: SingleChildScrollView(
          padding: const EdgeInsets.all(20),
          child: Container(
            width: double.infinity,
            padding: const EdgeInsets.all(20),
            margin: const EdgeInsets.symmetric(horizontal: 20),
            decoration: BoxDecoration(
              color: Colors.white,
              borderRadius: BorderRadius.circular(20),
              boxShadow: [
                BoxShadow(
                  color: Colors.grey.withOpacity(0.2),
                  spreadRadius: 5,
                  blurRadius: 10,
                  offset: const Offset(0, 5),
                ),
              ],
            ),
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.center,
              children: [
                const Text(
                  'Welcome to Parcel Shield',
                  style: TextStyle(
                    fontSize: 32,
                    fontWeight: FontWeight.bold,
                    color: Colors.blueAccent,
                  ),
                  textAlign: TextAlign.center,
                ),
                const SizedBox(height: 10),
                const Text(
                  'Secure your package with a One-Time Password (OTP).',
                  style: TextStyle(
                    fontSize: 18,
                    color: Colors.black54,
                  ),
                  textAlign: TextAlign.center,
                ),
                const SizedBox(height: 30),
                Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  mainAxisSize: MainAxisSize.min,
                  children: [
                    ConstrainedBox(
                      constraints: const BoxConstraints(maxWidth: 300), // Adjust width as needed
                      child: TextField(
                        onChanged: (value) => setState(() => _name = value),
                        decoration: const InputDecoration(
                          labelText: "Package Name",
                          border: OutlineInputBorder(),
                        ),
                      ),
                    ),
                    const SizedBox(width: 20),
                    ElevatedButton(
                      onPressed: _isLoading ? null : _requestOtp,
                      style: ElevatedButton.styleFrom(
                        backgroundColor: Colors.blueAccent,
                        padding: const EdgeInsets.symmetric(horizontal: 30, vertical: 15),
                        shape: RoundedRectangleBorder(
                          borderRadius: BorderRadius.circular(10),
                        ),
                        elevation: 5,
                        shadowColor: Colors.blue.withOpacity(0.5),
                      ),
                      child: _isLoading
                          ? const CircularProgressIndicator(
                              valueColor: AlwaysStoppedAnimation<Color>(Colors.white),
                            )
                          : const Text(
                              'Generate OTP',
                              style: TextStyle(fontSize: 18, color: Colors.white),
                            ),
                    ),
                  ],
                ),
                const SizedBox(height: 40),
                AnimatedOpacity(
                  opacity: _otp.isEmpty ? 0 : 1,
                  duration: const Duration(seconds: 1),
                  child: Container(
                    padding: const EdgeInsets.all(20),
                    decoration: BoxDecoration(
                      color: const Color(0xFFEDF2F7),
                      borderRadius: BorderRadius.circular(10),
                    ),
                    child: Text(
                      _otp.isNotEmpty ? 'Your OTP: $_otp' : 'OTP goes here',
                      style: TextStyle(
                        fontSize: 20,
                        color: _otp.isNotEmpty ? Colors.green : Colors.black45,
                        fontWeight: FontWeight.bold,
                      ),
                      textAlign: TextAlign.center,
                    ),
                  ),
                ),
                const SizedBox(height: 40),
                Container(
                  padding: const EdgeInsets.all(20),
                  decoration: BoxDecoration(
                    color: _hasPackage ? Colors.greenAccent.withOpacity(0.2) : Colors.redAccent.withOpacity(0.2),
                    borderRadius: BorderRadius.circular(10),
                  ),
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Icon(
                        _hasPackage ? Icons.check_circle : Icons.error,
                        color: _hasPackage ? Colors.green : Colors.red,
                        size: 30,
                      ),
                      const SizedBox(width: 10),
                      Text(
                        _hasPackage ? 'Package Present' : 'No Package Detected',
                        style: TextStyle(
                          fontSize: 18,
                          fontWeight: FontWeight.bold,
                          color: _hasPackage ? Colors.green : Colors.red,
                        ),
                      ),
                    ],
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
