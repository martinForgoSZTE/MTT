#pragma once

//the exe is supposed to be in bin/

#include <QString>
#include <QDir>
#include <QtCore>

inline constexpr qreal operator "" _qr(long double value){ return qreal(value); }

static const qreal CIRCLE_ITEM_WIDTH = 10;
static const qreal CIRCLE_ITEM_HEIGHT = 10;

static const QString& SQLITE_DRIVER = "QSQLITE";
static const QString& APPLICATION_NAME = "MTT - Szakdoga app";

static const QString& COMMON_DB_PATH = QDir::toNativeSeparators("../MTT/db/common.db");
static const QString& STORE_DB_PATH = QDir::toNativeSeparators("../MTT/db/store.db");
static const QString& STORE_DB_PREFIX = "store";
static const QString& STORE_CONN_NAME = "StoreConn";
static const QString& COMMON_CONN_NAME = "CommonConn";

static const QString& COUNTY_CSV_PATH = QDir::toNativeSeparators("../MTT/resources/counties.csv");
static const QString& COUNTY_SEAT_CSV_PATH = QDir::toNativeSeparators("../MTT/resources/county_seats.csv");
static const QString& REGION_CSV_PATH = QDir::toNativeSeparators("../MTT/resources/regions.csv");
static const QString& LARGE_REGION_CSV_PATH = QDir::toNativeSeparators("../MTT/resources/large_regions.csv");
constexpr std::size_t DELIMITER_ROW = 29;

static const QString& COUNTY_TABLE = "County";
static const QString& COUNTY_SEAT_TABLE = "County_Seat";
static const QString& REGIONS_TABLE = "Regions";
static const QString& LREGIONS_TABLE = "LRegions";

static const QString& CREATE_TABLE_COUNTY = "create table if not exists County("
                                                "name varchar(50),"
                                                "seat_name varchar(50)"
                                                ")";
static const QString& CREATE_TABLE_COUNTY_SEAT = "create table if not exists County_Seat("
                                                "name varchar(50),"
                                                "seat_name varchar(50),"
                                                "longitude real,"
                                                "latitude real"
                                                ")";
static const QString& CREATE_TABLE_REGIONS = "create table if not exists Regions("
                                            "name varchar(50),"
                                            "longitude real,"
                                            "latitude real"
                                            ")";
static const QString& CREATE_TABLE_LREGIONS = "create table if not exists LRegions("
                                              "name varchar(50),"
                                              "longitude real,"
                                              "latitude real"
                                              ")";

