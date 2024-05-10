#pragma once

#include "samples/opus/inline/simple/CompoundCTS.hpp"
#include "samples/opus/inline/simple/CompoundRTS.hpp"
#include "samples/opus/inline/simple/IntDouble.hpp"
#include "samples/opus/inline/simple/IntDoubleLong.hpp"
#include "samples/opus/inline/simple/Picture.hpp"
#include "samples/opus/inline/simple/Point.hpp"
#include "samples/opus/inline/simple/Rectangle.hpp"
#include "samples/opus/inline/simple/StringIntLong.hpp"

//statshouse
//common
#include "samples/opus/inline/statshouse/Dictionary.hpp"
#include "samples/opus/inline/statshouse/DictionaryField.hpp"
#include "samples/opus/inline/statshouse/True.hpp"

//public
#include "samples/opus/inline/statshouse/public/AddMetricsBatchQuery.hpp"    //
#include "samples/opus/inline/statshouse/public/Metric.hpp"

//api
#include "samples/opus/inline/statshouse/api/Filter.hpp"
#include "samples/opus/inline/statshouse/api/Flag.hpp"
#include "samples/opus/inline/statshouse/api/Function.hpp"
#include "samples/opus/inline/statshouse/api/GetChunkResponse.hpp"         //
#include "samples/opus/inline/statshouse/api/GetQueryPointResponse.hpp"    //
#include "samples/opus/inline/statshouse/api/GetQueryResponse.hpp"         //
#include "samples/opus/inline/statshouse/api/PointMeta.hpp"
#include "samples/opus/inline/statshouse/api/Query.hpp"                   //
#include "samples/opus/inline/statshouse/api/QueryPoint.hpp"              //
#include "samples/opus/inline/statshouse/api/ReleaseChunkResponse.hpp"    //
#include "samples/opus/inline/statshouse/api/Series.hpp"
#include "samples/opus/inline/statshouse/api/SeriesMeta.hpp"
#include "samples/opus/inline/statshouse/api/TagValue.hpp"

//engine
#include "samples/opus/inline/statshouse/engine/HttpQuery.hpp"            //
#include "samples/opus/inline/statshouse/engine/HttpQueryResponse.hpp"    //

//schema
#include "samples/opus/inline/statshouse/schema/Centroid.hpp"
#include "samples/opus/inline/statshouse/schema/IngestionStatus2.hpp"
#include "samples/opus/inline/statshouse/schema/MultiItem.hpp"
#include "samples/opus/inline/statshouse/schema/MultiValue.hpp"
#include "samples/opus/inline/statshouse/schema/SampleFactor.hpp"
#include "samples/opus/inline/statshouse/schema/SourceBucket2.hpp"    //

#include "samples/opus/inline/statshouse/HighLevelType.hpp"
