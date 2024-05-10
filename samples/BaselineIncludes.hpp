#pragma once

//simple
#include "samples/baseline/simple/CompoundCTS.hpp"
#include "samples/baseline/simple/CompoundRTS.hpp"
#include "samples/baseline/simple/IntDouble.hpp"
#include "samples/baseline/simple/IntDoubleLong.hpp"
#include "samples/baseline/simple/Picture.hpp"
#include "samples/baseline/simple/Point.hpp"
#include "samples/baseline/simple/Rectangle.hpp"
#include "samples/baseline/simple/StringIntLong.hpp"

//statshouse
//common
#include "samples/baseline/statshouse/Dictionary.hpp"
#include "samples/baseline/statshouse/DictionaryField.hpp"
#include "samples/baseline/statshouse/True.hpp"

//public
#include "samples/baseline/statshouse/public/AddMetricsBatchQuery.hpp"    //
#include "samples/baseline/statshouse/public/Metric.hpp"

//api
#include "samples/baseline/statshouse/api/Filter.hpp"
#include "samples/baseline/statshouse/api/Flag.hpp"
#include "samples/baseline/statshouse/api/Function.hpp"
#include "samples/baseline/statshouse/api/GetChunkResponse.hpp"         //
#include "samples/baseline/statshouse/api/GetQueryPointResponse.hpp"    //
#include "samples/baseline/statshouse/api/GetQueryResponse.hpp"         //
#include "samples/baseline/statshouse/api/PointMeta.hpp"
#include "samples/baseline/statshouse/api/Query.hpp"                   //
#include "samples/baseline/statshouse/api/QueryPoint.hpp"              //
#include "samples/baseline/statshouse/api/ReleaseChunkResponse.hpp"    //
#include "samples/baseline/statshouse/api/Series.hpp"
#include "samples/baseline/statshouse/api/SeriesMeta.hpp"
#include "samples/baseline/statshouse/api/TagValue.hpp"

//engine
#include "samples/baseline/statshouse/engine/HttpQuery.hpp"            //
#include "samples/baseline/statshouse/engine/HttpQueryResponse.hpp"    //

//schema
#include "samples/baseline/statshouse/schema/Centroid.hpp"
#include "samples/baseline/statshouse/schema/IngestionStatus2.hpp"
#include "samples/baseline/statshouse/schema/MultiItem.hpp"
#include "samples/baseline/statshouse/schema/MultiValue.hpp"
#include "samples/baseline/statshouse/schema/SampleFactor.hpp"
#include "samples/baseline/statshouse/schema/SourceBucket2.hpp"    //
#include "samples/baseline/statshouse/schema/TopElement.hpp"

#include "samples/baseline/statshouse/HighLevelType.hpp"
