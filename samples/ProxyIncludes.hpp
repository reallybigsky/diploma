#pragma once

#include "samples/opus/proxy/simple/CompoundCTS.hpp"
#include "samples/opus/proxy/simple/CompoundRTS.hpp"
#include "samples/opus/proxy/simple/IntDouble.hpp"
#include "samples/opus/proxy/simple/IntDoubleLong.hpp"
#include "samples/opus/proxy/simple/Picture.hpp"
#include "samples/opus/proxy/simple/Point.hpp"
#include "samples/opus/proxy/simple/Rectangle.hpp"
#include "samples/opus/proxy/simple/StringIntLong.hpp"

//statshouse
//common
#include "samples/opus/proxy/statshouse/Dictionary.hpp"
#include "samples/opus/proxy/statshouse/DictionaryField.hpp"
#include "samples/opus/proxy/statshouse/True.hpp"

//public
#include "samples/opus/proxy/statshouse/public/AddMetricsBatchQuery.hpp"    //
#include "samples/opus/proxy/statshouse/public/Metric.hpp"

//api
#include "samples/opus/proxy/statshouse/api/Filter.hpp"
#include "samples/opus/proxy/statshouse/api/Flag.hpp"
#include "samples/opus/proxy/statshouse/api/Function.hpp"
#include "samples/opus/proxy/statshouse/api/GetChunkResponse.hpp"         //
#include "samples/opus/proxy/statshouse/api/GetQueryPointResponse.hpp"    //
#include "samples/opus/proxy/statshouse/api/GetQueryResponse.hpp"         //
#include "samples/opus/proxy/statshouse/api/PointMeta.hpp"
#include "samples/opus/proxy/statshouse/api/Query.hpp"                   //
#include "samples/opus/proxy/statshouse/api/QueryPoint.hpp"              //
#include "samples/opus/proxy/statshouse/api/ReleaseChunkResponse.hpp"    //
#include "samples/opus/proxy/statshouse/api/Series.hpp"
#include "samples/opus/proxy/statshouse/api/SeriesMeta.hpp"
#include "samples/opus/proxy/statshouse/api/TagValue.hpp"

//engine
#include "samples/opus/proxy/statshouse/engine/HttpQuery.hpp"            //
#include "samples/opus/proxy/statshouse/engine/HttpQueryResponse.hpp"    //

//schema
#include "samples/opus/proxy/statshouse/schema/Centroid.hpp"
#include "samples/opus/proxy/statshouse/schema/IngestionStatus2.hpp"
#include "samples/opus/proxy/statshouse/schema/MultiItem.hpp"
#include "samples/opus/proxy/statshouse/schema/MultiValue.hpp"
#include "samples/opus/proxy/statshouse/schema/SampleFactor.hpp"
#include "samples/opus/proxy/statshouse/schema/SourceBucket2.hpp"    //
#include "samples/opus/proxy/statshouse/schema/TopElement.hpp"

#include "samples/opus/proxy/statshouse/HighLevelType.hpp"
