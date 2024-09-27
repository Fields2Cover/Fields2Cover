import fields2cover as f2c

def import_transport_lanes(transport_lanes: str) -> f2c.Cells:
    transport_lanes_import = f2c.Fields()
    f2c.Parser().importJsonFromString(transport_lanes, transport_lanes_import)
    transport_lanes_cells = f2c.Cells()

    for lane in transport_lanes_import:
        transport_lanes_cells.addGeometry(lane.getCellsAbsPosition().getGeometry(0))
    return transport_lanes_cells

def to_point(point):
    return f2c.Point(point.coordinates[0], point.coordinates[1])
