import 'package:flutter/material.dart';

void main() {
  runApp(const ParcelShieldApp());
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
  String _otp = '';
  bool _isLoading = false;
  bool _hasPackage = false; //placeholder for package status

  //function to simulate OTP request
  void _requestOtp() {
    setState(() {
      _isLoading = true;
    });

    Future.delayed(const Duration(seconds: 2), () {
      setState(() {
        _otp = '123456'; //this will be replaced with actual OTP logic.
        _isLoading = false;
      });
    });
  }

  //placeholder function to toggle package status
  void _togglePackageStatus() {
    setState(() {
      _hasPackage = !_hasPackage;
    });
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
                //header and description
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

                //request OTP Button
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
                const SizedBox(height: 40),

                //OTP Display Box
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

                //package Status Widget
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
                const SizedBox(height: 20),

                //button to simulate package status change 
                ElevatedButton(
                  onPressed: _togglePackageStatus,
                  style: ElevatedButton.styleFrom(
                    backgroundColor: Colors.grey,
                    padding: const EdgeInsets.symmetric(horizontal: 30, vertical: 15),
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(10),
                    ),
                  ),
                  child: const Text(
                    'click button to change package status, implement logic later',
                    style: TextStyle(fontSize: 16, color: Colors.white),
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

