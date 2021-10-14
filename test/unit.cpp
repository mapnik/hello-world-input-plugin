#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include <mapnik/datasource.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/util/fs.hpp>

// icu
#include <unicode/uclean.h>

TEST_CASE("datasources")
{

    SECTION("hello world")
    {

        std::string plugin("./hello.input");
        if (mapnik::util::exists(plugin))
        {
            try
            {
                mapnik::datasource_cache::instance().register_datasource(plugin);
                mapnik::parameters p;
                p["type"] = "hello";
                mapnik::datasource_ptr ds = mapnik::datasource_cache::instance().create(p);
                mapnik::box2d<double> bbox = ds->envelope();
                mapnik::query q(bbox);
                mapnik::featureset_ptr fs = ds->features(q);
                REQUIRE(fs != mapnik::featureset_ptr());
                mapnik::feature_ptr feat1 = fs->next();
                REQUIRE(feat1 != mapnik::feature_ptr());
                mapnik::feature_ptr feat2 = fs->next();
                REQUIRE(feat2 != mapnik::feature_ptr());
                REQUIRE(fs->next() == mapnik::feature_ptr());
                REQUIRE(feat1->id() == static_cast<mapnik::value_integer>(1));
                REQUIRE(feat2->id() == static_cast<mapnik::value_integer>(2));
                auto const &geom1 = feat1->get_geometry();
                REQUIRE(geom1.is<mapnik::geometry::point<double>>());
                auto const &point = mapnik::util::get<mapnik::geometry::point<double>>(geom1);
                REQUIRE(point.x == bbox.center().x);
                REQUIRE(point.y == bbox.center().y);
                auto const &geom2 = feat2->get_geometry();
                REQUIRE(geom2.is<mapnik::geometry::line_string<double>>());
                auto const &line = mapnik::util::get<mapnik::geometry::line_string<double>>(geom2);
                REQUIRE(line.size() == 4);
                REQUIRE(line[0].x == bbox.minx());
                REQUIRE(line[0].y == bbox.maxy());
            }
            catch (std::exception const &ex)
            {
                FAIL(ex.what());
            }
        }
        else
        {
            WARN(std::string("could not register ") + plugin);
        }
    }
}

int main(int argc, char *const argv[])
{
    int result = Catch::Session().run(argc, argv);
    // http://icu-project.org/apiref/icu4c/uclean_8h.html#a93f27d0ddc7c196a1da864763f2d8920
    u_cleanup();
    return result;
}
