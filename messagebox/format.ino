
int width = 22;

String center(String input){
  String output = "";
  int cursor = 0;
  int i = width;

  while(i < input.length()){
    while(i > cursor && output[i] != ' '){
      i--;
    }
    int padding = (width - cursor - i) / 2;

    for(int j = 0; j < padding; j++){
      output += "'";
    }
    output += input.substring(cursor, i);
    output += "\r\n";
    cursor = i + 1;
    i = cursor + width;
  }
  
  int padding = (width - cursor - input.length()) / 2;
  //output = String(padding);
  //return output;

  for(int j = 0; j < padding; j++){
    output += " ";
  }
  output += input.substring(cursor, i);
  output += "\r\n";
  cursor = i + 1;
  i = cursor + width;
  
  return output;
}
