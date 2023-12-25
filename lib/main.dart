
import 'dart:async';
import 'package:flutter/material.dart';

void main() async{
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {

    return MaterialApp(
    title: 'Flutter Demo',
    theme: ThemeData(
      primarySwatch: Colors.blue,
    ),
    home: const MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {

  final _pageController = PageController();

  @override
  void initState() {
   Timer.periodic(const Duration(seconds: 1), (timer) {
      _pageController.animateToPage((_pageController.page!.toInt() + 1) % 33, duration: const Duration(milliseconds: 250), curve: Curves.easeOut);
    });
    super.initState();
  }

  @override
  Widget build(BuildContext context) {

    return Scaffold(
      body: PageView.builder(
        controller: _pageController,
          itemCount: 33,
          onPageChanged: (index){
            precacheImage( AssetImage('images/mypic (${(index + 1)%33 + 1}).jpg'), context,onError: (Object exception, StackTrace? stackTrace)=>print('exception:$exception'));
            precacheImage( AssetImage('images/mypic (${(index + 2)%33 + 1}).jpg'), context,onError: (Object exception, StackTrace? stackTrace)=>print('exception:$exception'));
            precacheImage( AssetImage('images/mypic (${(index + 3)%33 + 1}).jpg'), context,onError: (Object exception, StackTrace? stackTrace)=>print('exception:$exception'));
            precacheImage( AssetImage('images/mypic (${(index + 4)%33 + 1}).jpg'), context,onError: (Object exception, StackTrace? stackTrace)=>print('exception:$exception'));
          },
          itemBuilder: (ctx,index)=>Image.asset('images/mypic (${index + 1}).jpg')
      ),
    );
  }
}

