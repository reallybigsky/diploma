#pragma once

#include "samples/opus/proxy_naive/simple/CompoundCTS.hpp"
#include "samples/opus/proxy_naive/simple/CompoundRTS.hpp"
#include "samples/opus/proxy_naive/simple/IntDouble.hpp"
#include "samples/opus/proxy_naive/simple/IntDoubleLong.hpp"
#include "samples/opus/proxy_naive/simple/Picture.hpp"
#include "samples/opus/proxy_naive/simple/Point.hpp"
#include "samples/opus/proxy_naive/simple/Rectangle.hpp"
#include "samples/opus/proxy_naive/simple/StringIntLong.hpp"

//statshouse
//common
#include "samples/opus/proxy_naive/statshouse/Dictionary.hpp"
#include "samples/opus/proxy_naive/statshouse/DictionaryField.hpp"
#include "samples/opus/proxy_naive/statshouse/True.hpp"

//public
#include "samples/opus/proxy_naive/statshouse/public/AddMetricsBatchQuery.hpp"    //
#include "samples/opus/proxy_naive/statshouse/public/Metric.hpp"

//api
#include "samples/opus/proxy_naive/statshouse/api/Filter.hpp"
#include "samples/opus/proxy_naive/statshouse/api/Flag.hpp"
#include "samples/opus/proxy_naive/statshouse/api/Function.hpp"
#include "samples/opus/proxy_naive/statshouse/api/GetChunkResponse.hpp"         //
#include "samples/opus/proxy_naive/statshouse/api/GetQueryPointResponse.hpp"    //
#include "samples/opus/proxy_naive/statshouse/api/GetQueryResponse.hpp"         //
#include "samples/opus/proxy_naive/statshouse/api/PointMeta.hpp"
#include "samples/opus/proxy_naive/statshouse/api/Query.hpp"                   //
#include "samples/opus/proxy_naive/statshouse/api/QueryPoint.hpp"              //
#include "samples/opus/proxy_naive/statshouse/api/ReleaseChunkResponse.hpp"    //
#include "samples/opus/proxy_naive/statshouse/api/Series.hpp"
#include "samples/opus/proxy_naive/statshouse/api/SeriesMeta.hpp"
#include "samples/opus/proxy_naive/statshouse/api/TagValue.hpp"

//engine
#include "samples/opus/proxy_naive/statshouse/engine/HttpQuery.hpp"            //
#include "samples/opus/proxy_naive/statshouse/engine/HttpQueryResponse.hpp"    //

//schema
#include "samples/opus/proxy_naive/statshouse/schema/Centroid.hpp"
#include "samples/opus/proxy_naive/statshouse/schema/IngestionStatus2.hpp"
#include "samples/opus/proxy_naive/statshouse/schema/MultiItem.hpp"
#include "samples/opus/proxy_naive/statshouse/schema/MultiValue.hpp"
#include "samples/opus/proxy_naive/statshouse/schema/SampleFactor.hpp"
#include "samples/opus/proxy_naive/statshouse/schema/SourceBucket2.hpp"    //
#include "samples/opus/proxy_naive/statshouse/schema/TopElement.hpp"

#include "samples/opus/proxy_naive/statshouse/HighLevelType.hpp"
