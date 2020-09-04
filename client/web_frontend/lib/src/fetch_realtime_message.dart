import 'dart:async';

void FetchRealTimeMessages(int interval) {
  Timer(Duration(seconds: interval), () {
    FetchRealTimeMessages(1);
  });
}
