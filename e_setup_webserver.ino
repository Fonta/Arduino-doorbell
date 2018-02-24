/************************* Webserver setup ******************************/
// Create the webserver
ESP8266WebServer webserver(80);
ESP8266HTTPUpdateServer httpUpdater;

const char* update_path = "/update";
// const char* update_username = "USERNAME";
// const char* update_password = "PASSWORD";

void setup_webserver() {

  //setup webserver addresses
  webserver.on("/", web_returnRoot);
  webserver.on("/upload", web_updatePage);
  webserver.on("/info", web_infoPage);
  webserver.on("/save", web_saveTimings);
  webserver.on("/default", web_defaultTimings);
  webserver.on("/reboot", web_rebootPage);

  webserver.onNotFound(web_handleNotFound);

  httpUpdater.setup(&webserver, update_path, update_username, update_password);
  webserver.begin();
}

String web_header() {
  String header = "<html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Doorbell Configuration</title>";
  header += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>";
  header += "</head><body>";
  header += "<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>Doorbell Configuration</a>";
  header += "<button class='navbar-toggler' type='button' data-toggle='collapse' data-target='#navbarCollapse' aria-controls='navbarCollapse' aria-expanded='false' aria-label='Toggle navigation'><span class='navbar-toggler-icon'></span></button>";
  header += "<div class='collapse navbar-collapse' id='navbarCollapse'><ul class='navbar-nav mr-auto'>";
  header += "<li class='nav-item'><a class='nav-link' href='/' id='homelink'>Home</a></li>";
  header += "<li class='nav-item'><a class='nav-link' href='/upload' id='updatelink'>Update</a></li>";
  header += "<li class='nav-item'><a class='nav-link' href='/info' id='infolink'>Info</a></li>";
  header += "</ul></div></div></nav>";

  return header;
}
String web_footer() {
  String footer = "<script src='https://code.jquery.com/jquery-3.2.1.slim.min.js' integrity='sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN' crossorigin='anonymous'></script>";
  footer += "<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.9/umd/popper.min.js' integrity='sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q' crossorigin='anonymous'></script>";
  footer += "<script src='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js' integrity='sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl' crossorigin='anonymous'></script>";
  footer += "<script>var pageid = $('main').attr('id'); $('a[id=' + pageid + 'link]').addClass('active');</script>";
  footer += "</body></html>";

  return footer;
}

void web_returnRoot() {
  String html = web_header();
  html += "<main role='main' class='container' id='home'><div class='jumbotron'>";
  html += "<form id='data' action='/save' method='POST'>";
  html += "<div class='form-group'><label for='debounceDurationField'>Debounce duration</label><input type='number' class='form-control' name='debounceDuration' id='debounceDurationField' placeholder='' value='";
  html += round(debounceDuration);
  html += "' ></div>";
  html += "<div class='form-group'><label for='bellOffDurationField'>Bell off duration</label><input type='number' class='form-control' name='bellOffDuration' id='bellOffDurationField' placeholder='' value='";
  html += round(bellOffDuration);
  html += "' ></div>";
  html += "<div class='form-group'><label for='noNotificationsDurationField'>Notification Cooldown duration</label><input type='number' class='form-control' name='noNotificationsDuration' id='noNotificationsDurationField' placeholder='' value='";
  html += round(noNotificationsDuration);
  html += "' ></div>";
  html += "<button type='submit' class='btn btn-primary'>Save!</button> <button type='button' class='btn btn-secondary' onclick='window.location.href=\"/default\"'>Defaults</button>";
  html += "</form></div></main>";
  html += web_footer();

  webserver.send(200, "text/html", html);
}

void web_infoPage() {
  String html = web_header();
  html += "<main role='main' class='container' id='info'><div class='jumbotron'>";
  html += "<p>Info page</p>";
  html += "<button type='button' class='btn btn-warning' onclick='window.location.href=\"/reboot\"'>Reboot</button>";
  html += "</div></main>";
  html += web_footer();

  webserver.send(200, "text/html", html);
}

void web_updatePage() {
  String html = web_header();
  html += "<main role='main' class='container' id='update'><div class='jumbotron'>";
  html += "<form id='data' action='/update' method='POST' enctype='multipart/form-data'>";
  html += "<div class='input-group mb-3'><div class='custom-file'><input type='file' accept='.bin' class='custom-file-input' id='inputGroupFile04' name='update' required><label class='custom-file-label' for='inputGroupFile04'>Choose file</label></div><div class='input-group-append'><button class='btn btn-danger' type='submit'>Update!</button></div></div>";
  html += "</form></div></main>";
  html += web_footer();
  html += "<script>$('.custom-file-input').on('change', function(e) { let fileName = $(this).val().split('\\\\').pop(); $(this).next('.custom-file-label').addClass('selected').html(fileName); });</script>";

  webserver.send(200, "text/html", html);
}

void web_rebootPage() {  
  String html = web_header();
  html += "<main role='main' class='container' id='info'><div class='jumbotron'>";
  html += "Rebooting the device!";
  html += "</div></main>";
  html += web_footer();
  html += "<script>setTimeout(function() { window.location.href = '/'; }, 3000);</script>";

  webserver.send(200, "text/html", html);
  ESP.restart();
}

void web_handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += webserver.uri();
  message += "\nMethod: ";
  message += (webserver.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += webserver.args();
  message += "\n";
  for (uint8_t i = 0; i < webserver.args(); i++) {
    message += " " + webserver.argName(i) + ": " + webserver.arg(i) + "\n";
  }

  webserver.send(404, "text/plain", message);
}

void web_saveTimings() {
  String str_debounceDuration = String(webserver.arg("debounceDuration"));
  String str_bellOffDuration = String(webserver.arg("bellOffDuration"));
  String str_noNotificationsDuration = String(webserver.arg("noNotificationsDuration"));

  if (debounceDuration != string_convert(str_debounceDuration))
  {
    debounceDuration = string_convert(str_debounceDuration);
    str_debounceDuration += ";";
    write_EEPROM(str_debounceDuration, 0);

    char charDebounceDuration[6];
    itoa(debounceDuration, charDebounceDuration, 10);
    mqttClient.publish(dbSetDebounceDuration, charDebounceDuration);

    printMsg("Saved new debounce duration value!");
  }

  if (bellOffDuration != string_convert(str_bellOffDuration))
  {
    bellOffDuration = string_convert(str_bellOffDuration);
    str_bellOffDuration += ";";
    write_EEPROM(str_bellOffDuration, 100);

    char charBellOffDuration[6];
    itoa(bellOffDuration, charBellOffDuration, 10);
    mqttClient.publish(dbSetbellOffDuration, charBellOffDuration);

    printMsg("Saved new bell off duration value!");
  }

  if (noNotificationsDuration != string_convert(str_noNotificationsDuration))
  {
    noNotificationsDuration = string_convert(str_noNotificationsDuration);
    str_noNotificationsDuration += ";";
    write_EEPROM(str_noNotificationsDuration, 200);

    char charNoNotificationsDuration[6];
    itoa(noNotificationsDuration, charNoNotificationsDuration, 10);
    mqttClient.publish(dbSetnoNotificationsDuration, charNoNotificationsDuration);

    printMsg("Saved new no notification duration value!");
  }

  EEPROM.commit();

  //write_to_Memory(str_debounceDuration, str_bellOffDuration, str_noNotificationsDuration);
  return web_returnRoot();
}

void web_defaultTimings() {
  debounceDuration = 50;
  bellOffDuration = 5000;
  noNotificationsDuration = 10000;

  String str_debounceDuration = String(debounceDuration);
  str_debounceDuration += ";";
  String str_bellOffDuration = String(bellOffDuration);
  str_bellOffDuration += ";";
  String str_noNotificationsDuration = String(noNotificationsDuration);
  str_noNotificationsDuration += ";";

  write_EEPROM(str_debounceDuration, 0);
  write_EEPROM(str_bellOffDuration, 100);
  write_EEPROM(str_noNotificationsDuration, 200);

  EEPROM.commit();

  return web_returnRoot();
}
