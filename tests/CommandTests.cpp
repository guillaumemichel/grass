#include "CommandTests.h"

void testCorrectIP(){
  int test_n = 10;
  string ips[test_n] = {"10.10.10.10", "0.0.0.0", "255.0.225.0", "255.255.255.255",
  "10.45.23.45", "123.124.254.231", "29.1.34.4", "100.200.0.0", "10.40.42.253", "200.45.12.7"};
  int i=0;
  for(;i<test_n;++i){
    assert(ips[i]==tokenize_ip(ips[i]));
  }
}

void testIncorrectIP(){
  int test_n = 10;
  string ips[test_n] = {"1000.10.10.10", "100.-1.0.0", "256.0.225.0", "255.255.255.",
  "10.45.23.405", "123 124 254 231", "29...1.34.4", "0x160.200.0.0", "", "12.7"};
  int i=0;
  for(;i<test_n;++i){
    try{
      tokenize_ip(ips[i]);
      assert(false);
    } catch(Exception e){
      assert(true);
    }

  }
}
