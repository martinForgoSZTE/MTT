#include "coords_chain_of_resp.h"

CoordinateProcessor::CoordinateProcessor(DB_Manager& manager, const QString& table) : db_manager(manager), table(table)
{}

void CoordinateProcessor::Process(QVector<Coordinate>& coordinates)
{
    auto processed_coords = ProcessImplementation(coordinates);
    if(next.get())
    {
        next->Process(processed_coords);
    }
}

void CoordinateProcessor::SetNext(std::unique_ptr<CoordinateProcessor> nextProcess)
{
    next = std::move(nextProcess);
}

QVector<Coordinate> GetGeoCoordinates::ProcessImplementation(QVector<Coordinate> coordinates)
{
    return db_manager.GetGeoCoordinates(table);
}

QVector<Coordinate> GetEndCoordinates::ProcessImplementation(QVector<Coordinate> coordinates)
{

}

QVector<Coordinate> MapCoordinates::ProcessImplementation(QVector<Coordinate> coordinates)
{

}
