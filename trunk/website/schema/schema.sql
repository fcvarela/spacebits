create table twits (
  change int,
  description char(160),
  link char(100), PRIMARY KEY(change)
  );

create table articles (
  id char(60),
  body blob,
  title char(80),
  change int,
  user char(20), PRIMARY KEY(id)
  );

create table articles_tags (
  id char(60),
  tag char(20), PRIMARY KEY(id,tag)
  );

create table articles_history (
  id char(60),
  body blob,
  title char(80),
  change int,
  user char(20), PRIMARY KEY(id,change)
  );

create table users (
  user char(60),
  password char(40),PRIMARY KEY(user)
  );

create table data (
  balloon int,
  change int,
  power_current real,
  power_voltage real,
  pressure real,
  temperature real,
  temperature_ext real,
  humidity real,
  dust_density real,
  lat real,
  lon real,
  alt real,
  bear real,
  imu_gx real, 
  imu_gy real, 
  imu_ax real, 
  imu_ay real, 
  imu_az real, PRIMARY KEY(change)
  );

create table sms (
  balloon int,
  change int,
  source char(80),
  message char(160),
  broadcast int, PRIMARY KEY(change)
  );

CREATE INDEX message_idx on sms (message);

